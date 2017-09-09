/* =====================================================================
 * Tic tac toe, what do you want?
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 3

#define TRUE 1
#define FALSE 0

#define EMPTY ' '

#define PONE 'O'
#define PTWO 'X'

// NOTE: row and col are indexed from 1, x and y are indexed from 0

struct GameState {
  char board[LENGTH * LENGTH];
  char currentPlayer;
};

// Clears the board in the Game State
void clearBoard(struct GameState *gs);

// Clears the board and sets player to player one
void init(struct GameState *gs);

// Draws the current board
void drawBoard(struct GameState *gs);

// Checks if move is valid
int isValid(struct GameState *gs, int x, int y);

// Verifies and then makes a move
// Does NOT change player on successful move
void makeMove(struct GameState *gs, char player, int row, int col);

// Checks if victory conditions are met on current board
int gameOver(struct GameState *gs);