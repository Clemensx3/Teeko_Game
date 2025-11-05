#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "framework.h"

#define BOARD_SIZE (5)

void welcome(void)
{
    printf("===| Welcome to       |===\n");
    printf("===| [I]ntricate      |===\n");
    printf("===| [S]quare         |===\n");
    printf("===| [P]lacement      |===\n");
    printf("\n");
}

void remainingPieces(int player_1_pieces, int player_2_pieces)
{
    printf("---| Remaining Pieces |---\n");
    printf("\n");
    printf("Player 1:  %d\n", player_1_pieces);
    printf("Player 2:  %d\n", player_2_pieces);
    printf("\n");
}

void returnPosition(char *placing_position, int *row, int *column)
{
    *row = placing_position[1] - '1';
    *column = toupper(placing_position[0]) - 'A';
}

bool checkIfCoordinatesOccupiedAndWithinBoard(int size, int board[size][size], int row, int column)
{
    if (row < 0 || row >= size || column < 0 || column >= size)
    {
      printf("[ERROR] Coordinates not within board.\n");
      printf("\n");
      return false;
    }
    if (board[row][column] != 0)
    {
      printf("[ERROR] Field at coordinates occupied.\n");
      printf("\n");
      return false;
    }
    return true;
}

void placingPhase(int size, int board[size][size], bool color_status, int player_1, int player_2)
{
    int player_1_pieces = 4;
    int player_2_pieces = 4;
    int change_player = 2;

    while(player_1_pieces != -1 && player_2_pieces != -1)
    {
      //Last two positions needed for \n and \0
      char placing_position[5];
      int row = 0;
      int column = 0;
      printBoard(size, board, color_status, player_1, player_2);
      remainingPieces(player_1_pieces, player_2_pieces);
      if(change_player % 2 == 0)
      {
        printf("P1 > ");
        fgets(placing_position, 5, stdin);
        if(strcmp(placing_position, "quit") == 0) {break;}
        printf("\n");
        returnPosition(placing_position, &row, &column);

        if(checkIfCoordinatesOccupiedAndWithinBoard(size, board, row, column))
        {
          board[row][column] = player_1;
          change_player++;
          player_1_pieces--;
        }
        else
        {         
          continue;
        }
        
      }
      else
      {
        printf("P2 > ");       
        fgets(placing_position, 5, stdin);
        if(strcmp(placing_position, "quit") == 0) {break;}
        printf("\n");
        returnPosition(placing_position, &row, &column);

        if(checkIfCoordinatesOccupiedAndWithinBoard(size, board, row, column))
        {
          board[row][column] = player_2;
          change_player++;
          player_2_pieces--;
        }
        else
        {
          continue;
        }
        
      }     

    }

}

int main(void)
{
    int board[BOARD_SIZE][BOARD_SIZE] = {0};
    welcome();
    placingPhase(BOARD_SIZE, board, false, 1, 2);
    //printBoard(5, board, false, 1, 2);
}
