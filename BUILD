cc_binary(
  name = "libprotocol_tic_tac_toe.so",
  srcs = ["protocol_tic_tac_toe.c"],
  deps = [
    ":ttt",
    "@libwebsockets//:libwebsockets",
  ],
  linkshared=1,
)

cc_library(
  name = "ttt",
  hdrs = ["ttt.h"],
  srcs = ["ttt.c"],
)