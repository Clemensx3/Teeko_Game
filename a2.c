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

bool checkIfLetter(int userInput)
{
    if((userInput >= 'A' && userInput <= 'Z') || (userInput >= 'a' && userInput <= 'z'))
    {
      return true;
    }
    return false;
}

bool checkIfNumber(int userInput)
{
    if(userInput >= '0' && userInput <= '9')
    {
      return true;
    }
    return false;
}

bool checkIfUserQuit(char* userQuit, int size)
{
    for(int i = 0; i < size; i++)
    {
      userQuit[i] = tolower(userQuit[i]);
    }

    if(strcmp(userQuit, "quit") == 0)
    {
      return true;
    }

    return false;
}

int getInput(char* input, int size, int* pointer_command_counter)
{
    int userInput = 0;
    int i = 0;
    int k = -1;
    bool last_char_was_letter = false;
    bool invalid_command = false;
    bool expected_space = false;
    char userQuit[size];
    do
    {
      userInput = getchar();

      if (userInput == EOF)
      {
        return -1;
      }
      if(checkIfLetter(userInput))
      {
        k++;
        userQuit[k] = (char)userInput;
      }

      if((*pointer_command_counter) > 2)
      {
        invalid_command = true;
      }
      else if(last_char_was_letter && userInput == ' ')
      {
        invalid_command = true;
      }
      else if(userInput == ' ')
      {
        expected_space = false;
      }
      else if(checkIfLetter(userInput))
      {
        if(last_char_was_letter == false && expected_space == false)
        {
          input[i] = (char)userInput;
          i++;
          last_char_was_letter = true;
          expected_space = true;
        }
        else
        {
          invalid_command = true;
        }
      }
      else if(checkIfNumber(userInput))
      {
        if(last_char_was_letter)
        {
          input[i] = (char)userInput;
          i++;
          last_char_was_letter = false;
          (*pointer_command_counter)++;
          expected_space = true;
        }
        else
        {
          invalid_command = true;
        }
      }        

    } while(userInput != '\n');
    if(checkIfUserQuit(userQuit, size))
    {
      return -1;
    }
    if(invalid_command || last_char_was_letter)
    {
      return 0;
    }
    return 1;
}

int choosingPosition(int size, int board[size][size], int player, int* pieces_left)
{
    //Last two positions needed for \n and \0
    char placing_position[10];
    int row = 0;
    int column = 0;
    int command_counter = 0;
    printf("P%d > ", player);
    int status = getInput(placing_position, 10, &command_counter);
    if(command_counter > 1)
    {
      printf("\n");
      printf("[ERROR] You must first place all your remaining pieces.\n");
      printf("\n");
      return 0;
    }
    if(status == 0)
    {
      printf("\n");
      printf("[ERROR] Unknown command.\n");
      printf("\n");
      return 0;
    }
    if(status == -1)
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
        if(move_counter % 2 == 0 && player_2_pieces > 0)
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

int checkCommandLineArgument(int argc ,char* argv)
{
    char lowercase_copy[argc];
    strcpy(lowercase_copy, argv);
    for(int i = 0; i < argc; i++)
    {
      lowercase_copy[i] = tolower(argv[i]);
    }

    if(strcmp(lowercase_copy, "enabled") == 0)
    {
      return 1;
    }
    else if((strcmp(lowercase_copy, "disabled") == 0))
    {
      return 0;
    }
    else
    {
      return -1;
    }
}

int returnCommandLineArgument(int argc, char* argv[])
{
    if(argc > 2)
    {
      printf("\n");
      printf("[ERROR] Usage: ./a2 [optional: color_enabled]\n");
      printf("\n");
      return -1;
    }

    int mode = 0;
    if(argc == 1)
    {
      mode = 0;
    }
    else 
    {
      mode = checkCommandLineArgument(argc, argv[1]);
    }
    
    if(mode == 1)
    {
      return 1;
    }
    else if(mode == 0)
    {
      return 0;
    }
    else 
    {
      printf("\n");
      printf("[ERROR] (Optional) color output argument can either be ""disabled"" or ""enabled"".\n");
      printf("\n");
      return -1;
    }
}

int main(int argc, char* argv[])
{
    int state = returnCommandLineArgument(argc, argv);
    if(state == -1)
    {
      return -1;
    }
    else
    {
      int board[BOARD_SIZE][BOARD_SIZE] = {0};
      welcome();
      placingPhase(BOARD_SIZE, board, state, 1, 2);
    }   
}
