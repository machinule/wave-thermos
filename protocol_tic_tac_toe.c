/* =====================================================================
 * Websockets protocol speaking tic tac toe
 * =====================================================================
 */
#define LWS_DLL
#define LWS_INTERNAL
#define LWS_WITH_PLUGINS
#include "external/libwebsockets/lib/libwebsockets.h"
#include "ttt.h"

#include <stdio.h>
#include <string.h>

struct per_vhost_data__tic_tac_toe {
  uv_timer_t timeout_watcher;
  struct lws_context *context;
  struct lws_vhost *vhost;
  const struct lws_protocols *protocol;
  struct GameState game;
};

struct per_session_data__tic_tac_toe {};

static void uv_timeout_cb_tic_tac_toe(uv_timer_t *w
#if UV_VERSION_MAJOR == 0
                                      ,
                                      int status
#endif
                                      ) {
  struct per_vhost_data__tic_tac_toe *vhd =
      lws_container_of(w, struct per_vhost_data__tic_tac_toe, timeout_watcher);
  lws_callback_on_writable_all_protocol_vhost(vhd->vhost, vhd->protocol);
}

static int callback_tic_tac_toe_protocol_init(struct lws *wsi) {
  struct per_vhost_data__tic_tac_toe *vhd =
      lws_protocol_vh_priv_zalloc(lws_get_vhost(wsi), lws_get_protocol(wsi),
                                  sizeof(struct per_vhost_data__tic_tac_toe));
  vhd->context = lws_get_context(wsi);
  vhd->protocol = lws_get_protocol(wsi);
  vhd->vhost = lws_get_vhost(wsi);
  uv_timer_init(lws_uv_getloop(vhd->context, 0), &vhd->timeout_watcher);
  uv_timer_start(&vhd->timeout_watcher, uv_timeout_cb_tic_tac_toe, 50, 50);
  init(&vhd->game);
  return 0;
}

static int callback_tic_tac_toe_protocol_destroy(struct lws *wsi) {
  struct per_vhost_data__tic_tac_toe *vhd =
      (struct per_vhost_data__tic_tac_toe *)lws_protocol_vh_priv_get(
          lws_get_vhost(wsi), lws_get_protocol(wsi));
  if (!vhd) {
    return 0;
  }
  uv_timer_stop(&vhd->timeout_watcher);
  return 0;
}

static int callback_tic_tac_toe(struct lws *wsi,
                                enum lws_callback_reasons reason, void *user,
                                void *in, size_t len) {
  struct per_vhost_data__tic_tac_toe *vhd =
      (struct per_vhost_data__tic_tac_toe *)lws_protocol_vh_priv_get(
          lws_get_vhost(wsi), lws_get_protocol(wsi));
  unsigned char buf[LWS_PRE + 512];
  unsigned char *p = &buf[LWS_PRE];
  int n, m;

  switch (reason) {
  case LWS_CALLBACK_PROTOCOL_INIT:
    return callback_tic_tac_toe_protocol_init(wsi);

  case LWS_CALLBACK_PROTOCOL_DESTROY:
    return callback_tic_tac_toe_protocol_destroy(wsi);

  case LWS_CALLBACK_ESTABLISHED:
    break;

  case LWS_CALLBACK_RECEIVE:
    if (len != 10 || strncmp((const char *)in, "move ", 5) != 0) {
      lwsl_notice("bad request\n");
      break;
    }
    char player = ((const char *)in)[5];
    int row = ((const char *)in)[7] - '0';
    int col = ((const char *)in)[9] - '0';
    if (player != PONE && player != PTWO) {
      lwsl_notice("player is not 'X' or 'O'\n");
      break;
    }
    if (row < 0 || row > 2) {
      lwsl_notice("row is not between 0-2\n");
      break;
    }
    if (col < 0 || col > 2) {
      lwsl_notice("col is not between 0-2\n");
      break;
    }
    makeMove(&vhd->game, player, row + 1, col + 1);
    if (gameOver(&vhd->game) != 0) {
      n = sprintf((char *)p, "gameover");
      m = lws_write(wsi, p, n, LWS_WRITE_TEXT);
      if (m < n) {
        lwsl_err("ERROR %d writing to di socket\n", n);
        return -1;
      }
      init(&vhd->game);
    }
    drawBoard(&vhd->game);
    break;

  default:
    break;
  }

  return 0;
}

static const struct lws_protocols protocols[] = {
    {
        "tic-tac-toe-protocol", callback_tic_tac_toe,
        sizeof(struct per_session_data__tic_tac_toe),
        128, /* rx buf size must be >= permessage-deflate rx size */
    },
};

LWS_VISIBLE int init_protocol_tic_tac_toe(struct lws_context *context,
                                          struct lws_plugin_capability *c) {
  if (c->api_magic != LWS_PLUGIN_API_MAGIC) {
    lwsl_err("Plugin API %d, library API %d", LWS_PLUGIN_API_MAGIC,
             c->api_magic);
    return 1;
  }

  c->protocols = protocols;
  c->count_protocols = ARRAY_SIZE(protocols);
  c->extensions = NULL;
  c->count_extensions = 0;

  return 0;
}

LWS_VISIBLE int destroy_protocol_tic_tac_toe(struct lws_context *context) {
  return 0;
}