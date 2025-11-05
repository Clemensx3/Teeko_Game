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
    printf("Player 1: %d\n", player_1_pieces);
    printf("Player 2: %d\n", player_2_pieces);
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
      printf("\n");
      printf("[ERROR] Coordinates not within board.\n");
      printf("\n");
      return false;
    }
    if (board[row][column] != 0)
    {
      printf("\n");
      printf("[ERROR] Field at coordinates occupied.\n");
      printf("\n");
      return false;
    }
    return true;
}

bool getUserInput(char* input, int size)
{
    if(fgets(input, size, stdin) == NULL)
    {
      return false;
    }

    //Removes enter by getting index that = \n
    input[strcspn(input, "\n")] = '\0';

    char lowercase_copy[size];
    strcpy(lowercase_copy, input);
    for(int i = 0; i < size; i++)
    {
      lowercase_copy[i] = tolower(lowercase_copy[i]);
    }

    if(strcmp(lowercase_copy, "quit") == 0)
    {
      return false;
    }

    return true;
}

int choosingPosition(int size, int board[size][size], int player, int* pieces_left)
{
    //Last two positions needed for \n and \0
    char placing_position[10];
    int row = 0;
    int column = 0;
    printf("P%d > ", player);
    bool status = getUserInput(placing_position, 10);
    if(status == false)
    {
      return -1;
    }
    returnPosition(placing_position, &row, &column);

    if(checkIfCoordinatesOccupiedAndWithinBoard(size, board, row, column))
    {
      board[row][column] = player;
      (*pieces_left)--;
      return 1;
    }
    else
    {         
      return 0;
    }
}

void placingPhase(int size, int board[size][size], bool color_status, int player_1, int player_2)
{
    int player_1_pieces = 4;
    int player_2_pieces = 4;
    int move_counter = 0;
    int status = 1;

    while((player_1_pieces + player_2_pieces) > 0)
    {  
      if(status == 1)
      {
        if(move_counter % 2 == 0 && player_2_pieces > 1)
        {
          printf("\n");
          printf("===| Move %d: Player %d |===\n", 5-player_1_pieces, player_1);
          printf("\n");
        }
        else
        {
          printf("\n");
          printf("===| Move %d: Player %d |===\n", 5-player_2_pieces, player_2);
          printf("\n");
        }

        printBoard(size, board, color_status, player_1, player_2);
        remainingPieces(player_1_pieces, player_2_pieces);
      }

      if(move_counter % 2 == 0)
      {     
        status = choosingPosition(size, board, player_1, &player_1_pieces);
        if(status == 0)
        {
          continue;
        }
        else if(status == -1)
        {
          break;
        }
        status = 1;
        move_counter++;
      }
      else
      {     
        status = choosingPosition(size, board, player_2, &player_2_pieces);
        if(status == 0)
        {
          continue;
        }
        else if(status == -1)
        {
          break;
        }
        status = 1;
        move_counter++;
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
