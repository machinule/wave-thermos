/* =====================================================================
 * Tic tac toe, what do you want?
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ttt.h"

void clearBoard(struct GameState *gs) {
  for (int i = 0; i < (LENGTH * LENGTH); i++) {
    gs->board[i] = EMPTY;
  }
}

void init(struct GameState *gs) {
  gs->currentPlayer = PONE;
  clearBoard(gs);
}

void drawBoard(struct GameState *gs) {
  printf("\n");
  printf("\t  %c  |  %c  |  %c  \n", gs->board[6], gs->board[7], gs->board[8]);
  printf("\t ---------------- \n");
  printf("\t  %c  |  %c  |  %c  \n", gs->board[3], gs->board[4], gs->board[5]);
  printf("\t ---------------- \n");
  printf("\t  %c  |  %c  |  %c  \n", gs->board[0], gs->board[1], gs->board[2]);
  printf("\n");
}

int getInput() {
  char input[256];
  int result;
  if (fgets(input, sizeof(input), stdin) != NULL) {
    if (sscanf(input, "%d", &result)) {
      if (result == 0) {
        printf("Count like a normal person.\n");
      } else if (result > LENGTH) {
        printf("What is your problem?\n");
        result = 0;
      } else if (result < 1) {
        printf("Enter a coordinate, not your IQ.\n");
        result = 0;
      }
    } else {
      printf("C'mon, seriously?\n");
    }
  } else {
    printf("I don't even know how to trigger this error.\n");
  }

  return result;
}

int isValid(struct GameState *gs, int x, int y) {
  // Check if square is empty
  if (gs->board[x + y * 3] != EMPTY) {
    printf("Try being less of a jerk.\n");
    return FALSE;
  }
  return TRUE;
}

void makeMove(struct GameState *gs, char player, int row, int col) {

  if (player != gs->currentPlayer) {
    printf("Wrong player attempting to move.");
    return;
  }

  // Coords are adjusted to index zero
  int x = col - 1;
  int y = row - 1;

  if (gs->currentPlayer == PONE) {
    gs->board[(x) + (y)*3] = PONE;
    gs->currentPlayer = PTWO;
  } else if (gs->currentPlayer == PTWO) {
    gs->board[(x) + (y)*3] = PTWO;
    gs->currentPlayer = PONE;
  } else {
    printf("Game state not properly initialized.");
  }
}

int gameOver(struct GameState *gs) {
  if ((gs->board[0] == gs->board[4] && gs->board[0] == gs->board[8] &&
       gs->board[0] != EMPTY) ||
      (gs->board[2] == gs->board[4] && gs->board[2] == gs->board[6] &&
       gs->board[2] != EMPTY)) {
    return TRUE;
  } else {
    for (int line = 0; line <= 2; line++) {
      if ((gs->board[line] == gs->board[line + 3] &&
           gs->board[line] == gs->board[line + 6] &&
           gs->board[line] != EMPTY) ||
          (gs->board[line * 3] == gs->board[line * 3 + 1] &&
           gs->board[line * 3] == gs->board[line * 3 + 2] &&
           gs->board[line * 3] != EMPTY)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
