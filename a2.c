#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "framework.h"

#define BOARD_SIZE (5)
#define INPUT_SIZE (100)

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

void returnPositionPlacing(char *placing_position, int *row, int *column)
{
    *row = placing_position[1] - '1';
    *column = toupper(placing_position[0]) - 'A';
}

void returnPositionMoving(char *placing_position, int* moves)
{
    moves[0] = placing_position[1] - '1';
    moves[1] = toupper(placing_position[0]) - 'A';
    moves[2] = placing_position[3] - '1';
    moves[3] = toupper(placing_position[2]) - 'A';
}

bool checkIfCoordinatesWithinBoard(int size, int row, int column)
{
    if (row < 0 || row >= size || column < 0 || column >= size)
    {
      printf("\n");
      printf("[ERROR] Coordinates not within board.\n");
      printf("\n");
      return false;
    }

    return true;
}

bool checkIfCoordinatesOccupied(int size, int board[size][size], int row, int column)
{
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

int readUserInput(char* command_buffer, char* raw_buffer, 
                  bool* p_invalid_format, int* p_command_counter, 
                  bool* p_last_was_letter, bool* p_expect_space)
{
    int cmd_idx = 0;
    int raw_idx = 0;
    int userInput;
    bool leading_space = true;
    *p_command_counter = 0; 
    *p_expect_space = false; 

    do
    {
        userInput = getchar();
        if (userInput == EOF)
        {
            return -1;
        }

        if (leading_space && !isspace(userInput) && userInput != '\n')
        {
            leading_space = false;
        }
        if (!leading_space && userInput != '\n')
        {
            if (raw_idx < INPUT_SIZE - 1)
            {
                raw_buffer[raw_idx++] = (char)userInput;
            }
        }

        if (isspace(userInput) || userInput == '\n')
        {
            if (*p_last_was_letter)
            {
                *p_invalid_format = true;
            }
            *p_last_was_letter = false;

            if (*p_expect_space)
            {
                *p_expect_space = false;
            }
        }
        else if (checkIfLetter(userInput))
        {
            if (*p_expect_space)
            {
                *p_invalid_format = true;
            }
            
            if (*p_last_was_letter)
            {
                *p_invalid_format = true;
            }
            *p_last_was_letter = true;
            if (cmd_idx < INPUT_SIZE - 1)
            {
                command_buffer[cmd_idx++] = (char)userInput;
            }
        }
        else if (checkIfNumber(userInput))
        {
            if (!*p_last_was_letter)
            {
                *p_invalid_format = true; 
            }
            *p_last_was_letter = false;
            (*p_command_counter)++; 
            *p_expect_space = true;
            
            if (cmd_idx < INPUT_SIZE - 1)
            {
                command_buffer[cmd_idx++] = (char)userInput;
            }
        }
        else
        {
            *p_invalid_format = true; 
        }

    } while (userInput != '\n');
    
    command_buffer[cmd_idx] = '\0';
    raw_buffer[raw_idx] = '\0';
    return 1;
}

bool processQuitCheck(char* raw_buffer)
{
    int i = strlen(raw_buffer) - 1;
    while (i >= 0 && isspace(raw_buffer[i]))
    {
        raw_buffer[i--] = '\0';
    }
    
    for (i = 0; raw_buffer[i]; i++)
    {
        raw_buffer[i] = tolower(raw_buffer[i]);
    }

    if (strcmp(raw_buffer, "quit") == 0)
    {
        return true;
    }  
    return false;
}

int getInput(char* input, int size, int* pointer_command_counter)
{
    char command_buffer[INPUT_SIZE] = {0};
    char raw_buffer[INPUT_SIZE] = {0};
    
    bool invalid_format = false;
    bool last_was_letter = false;
    bool expect_space = false; 

    int read_status = readUserInput(command_buffer, raw_buffer, 
                                    &invalid_format, pointer_command_counter, 
                                    &last_was_letter, &expect_space);
    
    if (read_status == -1 || processQuitCheck(raw_buffer))
    {
        return -1;
    }

    if (invalid_format || last_was_letter || strlen(command_buffer) == 0)
    {
        return 0; 
    }

    strncpy(input, command_buffer, size - 1);
    input[size - 1] = '\0';
    
    return 1;
}

int placingPhase(char* user_input, int size, int board[size][size], int player, int* pieces_left)
{
    int row = 0;
    int column = 0;
    returnPositionPlacing(user_input, &row, &column);

    if(checkIfCoordinatesWithinBoard(size, row, column) && checkIfCoordinatesOccupied(size, board, row, column))
    {
      board[row][column] = player;
      pieces_left[player-1]--;
      return 1;
    }
    else
    {         
      return 0;
    }
}

bool isAllowedMove(int* moves)
{
    int allowed_moves[4][2] = {
      {moves[0] - 1, moves[1]},
      {moves[0] + 1, moves[1]},
      {moves[0], moves[1] - 1},
      {moves[0], moves[1] + 1}
    };

    for(int i = 0; i < 4; i++)
    {
      if(moves[3] == allowed_moves[i][1] && moves[2] == allowed_moves[i][0])
      {
        return true;
      }
    }
    return false;
}

int movingPhase(char* user_input, int size, int board[size][size], int player)
{
    int moves[] = {0, 0, 0, 0};
    returnPositionMoving(user_input, moves);

    //printf("\n Inside movingphase: %d %d %d %d \n", moves[0], moves[1], moves[2], moves[3]);
    if(checkIfCoordinatesWithinBoard(size, moves[0], moves[1]) == false || checkIfCoordinatesWithinBoard(size, moves[2], moves[3]) == false)
    {
      return 0;
    }
    if(board[moves[0]][moves[1]] != player)
    {
      printf("\n");
      printf("[ERROR] No shiftable piece at coordinates.\n");
      printf("\n");
      return 0;
    }
    if(checkIfCoordinatesOccupied(size, board, moves[2], moves[3]) == false)
    {
      return 0;
    }

    if(isAllowedMove(moves) == false)
    {
      printf("\n");
      printf("[ERROR] Field at coordinates not adjacent to chosen piece.\n");
      printf("\n");
      return 0;
    }

    board[moves[0]][moves[1]] = 0;
    board[moves[2]][moves[3]] = player;
    return 1;
}

int checkBoard(int size, int board[size][size], int player, int* pieces_left)
{
    //Last two positions needed for \n and \0
    char user_input[INPUT_SIZE];   
    int command_counter = 0;
    printf("P%d > ", player);
    int status = getInput(user_input, INPUT_SIZE, &command_counter);

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

    if((pieces_left[0] + pieces_left[1]) > 0)
    {
      if(command_counter > 1)
      {
        printf("\n");
        printf("[ERROR] You must first place all your remaining pieces.\n");
        printf("\n");
        return 0;
      }
      return placingPhase(user_input, size, board, player, pieces_left);

    }

    if((pieces_left[0] + pieces_left[1]) <= 0)
    {
      if(command_counter == 1)
      {
        printf("\n");
        printf("[ERROR] You must specify a piece to shift and a target field.\n");
        printf("\n");
        return 0;
      } 
      return movingPhase(user_input, size, board, player);   
    }
    
    //Might need to change
    return -1;
}

bool checkWin(int size, int board[size][size], int player)
{
    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            if (board[r][c] != player)
            {
                continue;
            }

            if (c <= size - 4)
            {
                if (board[r][c+1] == player &&
                    board[r][c+2] == player &&
                    board[r][c+3] == player)
                {
                    return true;
                }
            }

            if (r <= size - 4)
            {
                if (board[r+1][c] == player &&
                    board[r+2][c] == player &&
                    board[r+3][c] == player)
                {
                    return true;
                }
            }

            if (r < size - 1 && c < size - 1)
            {
                if (board[r][c+1]   == player &&
                    board[r+1][c]   == player &&
                    board[r+1][c+1] == player)
                {
                    return true;
                }
            }

            if (r <= size - 4 && c <= size - 4)
            {
                if (board[r+1][c+1] == player &&
                    board[r+2][c+2] == player &&
                    board[r+3][c+3] == player)
                {
                    return true;
                }
            }

            if (r >= 3 && c <= size - 4)
            {
                if (board[r-1][c+1] == player &&
                    board[r-2][c+2] == player &&
                    board[r-3][c+3] == player)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

int initializeGame(int size, int board[size][size], bool color_status, int player_1, int player_2)
{
    int current_player = 0;
    int player_current_pieces[] = {4,4};
    int move_counter = 0;
    int display_round = 0;
    int status = 1;

    while(1)
    {  
      current_player = (move_counter % 2) + 1;
      display_round = (move_counter / 2) + 1;
      if(status == 1)
      {
        printf("\n");
        printf("===| Move %d: Player %d |===\n", display_round, current_player);
        printf("\n");

        printBoard(size, board, color_status, player_1, player_2);
        if((player_current_pieces[0] + player_current_pieces[1]) > 0)
        {
          remainingPieces(player_current_pieces[0], player_current_pieces[1]);
        }
      }
   
      status = checkBoard(size, board, current_player, player_current_pieces);
      
      if(status == 0)
      {
        continue;
      }
      else if(status == -1)
      {
        break; 
      }

      if (checkWin(size, board, current_player))
      {
          printf("\n===| Final Game State |===\n");
          printf("\n");
          printBoard(size, board, color_status, player_1, player_2);
          
          printf("===| Winner: Player %d |===\n", current_player);
          
          return current_player;
      }

      status = 1; 
      move_counter++;
    }

    return 0;
}

int checkCommandLineArgument(char* argv)
{
    char lowercase_copy[INPUT_SIZE];
    strcpy(lowercase_copy, argv);
    int i = 0;
    while (argv[i]) 
    {
        lowercase_copy[i] = tolower(argv[i]);
        i++;
    }
    lowercase_copy[i] = '\0';

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
      mode = checkCommandLineArgument(argv[1]);
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
      printf("[ERROR] (Optional) color output argument can either be \"disabled\" or \"enabled\".\n");
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
      return initializeGame(BOARD_SIZE, board, state, 1, 2);
    }   
}
