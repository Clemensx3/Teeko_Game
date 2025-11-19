//------------------------------------------------------------------------------
// Game inspired by Teeko. 
// Two players play against each other and try to reach a winning pattern.
// Divided into placing phase and moving phase.
//
// Author: 12513567
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "framework.h"

#define BOARD_SIZE (5)
#define INPUT_SIZE (100)
#define PLAYER_ONE_ID (1)
#define PLAYER_TWO_ID (2)
#define PIECES_TO_PLACE (4)
#define WINNING_PIECES (4)

//----------------------------------------------------------------------------------------------------------------------
/// @brief Prints welcome message and game title
void printWelcome(void)
{
  printf("===| Welcome to       |===\n");
  printf("===| [I]ntricate      |===\n");
  printf("===| [S]quare         |===\n");
  printf("===| [P]lacement      |===\n\n");
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Displays the number of pieces remaining for both players
/// @param player_1_pieces Number of pieces Player 1 has left to place
/// @param player_2_pieces Number of pieces Player 2 has left to place
void printRemainingPieces(int player_1_pieces, int player_2_pieces)
{
  printf("---| Remaining Pieces |---\n");
  printf("\nPlayer 1: %d\n", player_1_pieces);
  printf("Player 2: %d\n\n", player_2_pieces);
}

/// @brief Converts input position into useable row and column position
/// @param placing_command String of users placing position input
/// @param row Pointer to store the calculated row index
/// @param column Pointer to store the calculated column index 
void getPositionPlacing(char *placing_command, int *row, int *column)
{
  *row = placing_command[1] - '1';
  *column = toupper(placing_command[0]) - 'A';
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Converts input position into useable row and column position
/// @param moving_command String of user move input
/// @param moves Array to store row and column from where to move to dedicated destination
void getPositionMoving(char *moving_command, int* moves)
{
  moves[0] = moving_command[1] - '1';
  moves[1] = toupper(moving_command[0]) - 'A';
  moves[2] = moving_command[3] - '1';
  moves[3] = toupper(moving_command[2]) - 'A';
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if position is within boundries of board
/// @param size Dimension of board
/// @param row Row that needs to be checked 
/// @param column Column that needs to be checked 
/// @return True if valid, false if not
bool checkIfCoordinatesWithinBoard(int size, int row, int column)
{
  if (row < 0 || row >= size || column < 0 || column >= size)
  {
    printf("\n[ERROR] Coordinates not within board.\n\n");
    return false;
  }

  return true;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief checks if position is already occupied by piece
/// @param size Dimensions of board
/// @param board Board array
/// @param row Row to check
/// @param column Column to check 
/// @return False if occupied and true if free
bool checkIfCoordinatesOccupied(int size, int board[size][size], int row, int column)
{
  if (board[row][column] != 0)
  {
    printf("\n[ERROR] Field at coordinates occupied.\n\n");
    return false;
  }
  return true;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if input is a letter
/// @param user_input Current input character
/// @return True if letter otherwise or false
bool checkIfLetter(int user_input)
{
  if ((user_input >= 'A' && user_input <= 'Z') || (user_input >= 'a' && user_input <= 'z'))
  {
    return true;
  }
  return false;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if input is a number
/// @param user_input Current input character 
/// @return True if letter otherwise or false
bool checkIfNumber(int user_input)
{
  if (user_input >= '0' && user_input <= '9')
  {
    return true;
  }
  return false;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Ignores leading whitepaces and copies every char after finding none space
/// @param user_input Current input
/// @param raw_buffer Array of input string 
/// @param p_raw_index Keeps track index in string array
/// @param p_leading_space Keeps track while user enters only spaces before first valid char
void cleanRawBuffer(int user_input, char* raw_buffer, int* p_raw_index, bool* p_leading_space)
{
  if (*p_leading_space && !isspace(user_input) && user_input != '\n')
  {
    *p_leading_space = false;
  }
  
  if (!*p_leading_space && user_input != '\n')
  {
    if (*p_raw_index < INPUT_SIZE - 1) 
    {
      raw_buffer[(*p_raw_index)++] = (char)user_input;
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Reads characters to build command and raw buffers while validating format
/// @param command_buffer Buffer to store game commands
/// @param raw_buffer Buffer to store raw input for quit check
/// @param p_invalid_format Pointer keep track if input is valid
/// @param p_command_counter Pointer to count valid coordinate pairs
/// @param p_last_was_letter Pointer to track if previous character was a letter
/// @param p_expect_space Pointer to track if a space is required before next command
/// @return -1 if EOF is encountered, 1 if the line was read successfully.
int readUserInput(char* command_buffer, char* raw_buffer, bool* p_invalid_format,
  int* p_command_counter, bool* p_last_was_letter, bool* p_expect_space)
{
  int command_index = 0;
  int raw_index = 0;
  int user_input;
  bool leading_space = true;
  *p_command_counter = 0;
  *p_expect_space = false;

  do
  {
    user_input = getchar();
    if (user_input == EOF)
    {
      return -1;
    }
    cleanRawBuffer(user_input, raw_buffer, &raw_index, &leading_space);

    if (isspace(user_input) || user_input == '\n')
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
    else if (checkIfLetter(user_input))
    {
      if (*p_expect_space || *p_last_was_letter) 
      {
        *p_invalid_format = true;
      }
      *p_last_was_letter = true;
      if (command_index < INPUT_SIZE - 1)
      {
        command_buffer[command_index++] = (char)user_input;
      }
    }
    else if (checkIfNumber(user_input))
    {
      if (!*p_last_was_letter)
      {
        *p_invalid_format = true; 
      }
      *p_last_was_letter = false;
      (*p_command_counter)++; 
      *p_expect_space = true;
        
      if (command_index < INPUT_SIZE - 1)
      {
        command_buffer[command_index++] = (char)user_input;
      }
      }
      else
      {
        *p_invalid_format = true; 
      }
  } while (user_input != '\n');
  
  command_buffer[command_index] = '\0';
  raw_buffer[raw_index] = '\0';
  return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if raw user input matches "quit" (case insensitive)
/// @param raw_buffer The input string, which will be trimmed and lowercased.
/// @return True if the input is "quit", false otherwise
bool checkQuit(char* raw_buffer)
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

//----------------------------------------------------------------------------------------------------------------------
/// @brief Reads a line, checks for quit, and validates commands
/// @param input The destination buffer where valid and clean command will be copied
/// @param size The size of the input buffer
/// @param pointer_command_counter Pointer to store the number of valid commands found
/// @return -1 if user quits or EOF, 0 if input is invalid, 1 if input is valid
int getInput(char* input, int size, int* pointer_command_counter)
{
  char command_buffer[INPUT_SIZE] = {0};
  char raw_buffer[INPUT_SIZE] = {0};
  
  bool invalid_format = false;
  bool last_was_letter = false;
  bool expect_space = false;

  int read_status = readUserInput(command_buffer, raw_buffer, &invalid_format,
    pointer_command_counter, &last_was_letter, &expect_space);
  
  if (read_status == -1 || checkQuit(raw_buffer))
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

//----------------------------------------------------------------------------------------------------------------------
/// @brief Executes the logic for the "Placement Phase"
///        Updates the board and decreases pieces left
/// @param user_input Clean command string containing coordinates
/// @param size The dimension the board
/// @param board The game board array
/// @param player Current player
/// @param pieces_left Array tracking number of pieces remaining for both players
/// @return 1 if the piece was successfully placed, 0 if the move was invalid.
int placingPhase(char* user_input, int size, int board[size][size], int player, int* pieces_left)
{
  int row = 0;
  int column = 0;
  getPositionPlacing(user_input, &row, &column);

  if (checkIfCoordinatesWithinBoard(size, row, column) && checkIfCoordinatesOccupied(size, board, row, column))
  {
    board[row][column] = player;
    pieces_left[player-1]--;
    return 1;
  }
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Verifies if move is valid by checking if the destination is adjacent
/// @param player_move Array containing move command coordinates
/// @return true if the target is exactly one step away from the source, false otherwise.
bool checkAllowedMove(int* player_move)
{
  int row_player = player_move[0];
  int column_player = player_move[1];

  int allowed_moves[8][2] = {
    {row_player - 1, column_player},
    {row_player + 1, column_player},
    {row_player, column_player - 1},
    {row_player, column_player + 1},
    {row_player - 1, column_player - 1},
    {row_player - 1, column_player + 1}, 
    {row_player + 1, column_player - 1},
    {row_player + 1, column_player + 1}
  };

  for (int i = 0; i < 8; i++)
  {
    if(player_move[3] == allowed_moves[i][1] && player_move[2] == allowed_moves[i][0])
    {
      return true;
    }
  }
  return false;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Executes the logic for the "Movement Phase" amd updates the board
/// @param user_input Clean command string containing coordinates
/// @param size The dimension the board
/// @param board The game board array
/// @param player Current player
/// @return 1 if the piece was successfully moved, 0 if the move was invalid.
int movingPhase(char* user_input, int size, int board[size][size], int player)
{
  int player_move[] = {0, 0, 0, 0};
  getPositionMoving (user_input, player_move);

  if (checkIfCoordinatesWithinBoard(size, player_move[0], player_move[1]) == false ||
      checkIfCoordinatesWithinBoard(size, player_move[2], player_move[3]) == false)
  {
    return 0;
  }
  if (board[player_move[0]][player_move[1]] != player)
  {
    printf("\n[ERROR] No shiftable piece at coordinates.\n\n");
    return 0;
  }
  if (checkIfCoordinatesOccupied(size, board, player_move[2], player_move[3]) == false)
  {
    return 0;
  }

  if (checkAllowedMove(player_move) == false)
  {
    printf("\n[ERROR] Field at coordinates not adjacent to chosen piece.\n\n");
    return 0;
  }

  board[player_move[0]][player_move[1]] = 0;
  board[player_move[2]][player_move[3]] = player;
  return 1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Starts a players turn and determines wether its placing of moving phase
///        Also validates if the command count matches the current phase requirements
/// @param size The dimension the board
/// @param board The game board array
/// @param player Current player
/// @param pieces_left Array containing the count of pieces remaining for both players
/// @return 1 if a valid move was made, 0 if an error occurred (invalid command/logic), or -1 for Quit/EOF
int checkBoard(int size, int board[size][size], int player, int* pieces_left)
{
  char user_input[INPUT_SIZE];
  int command_counter = 0;
  printf("P%d > ", player);
  int status = getInput(user_input, INPUT_SIZE, &command_counter);

  if (status == 0)
  {
    printf("\n[ERROR] Unknown command.\n\n");
    return 0;
  }

  if (status == -1)
  {
    return -1;
  }

  if ((pieces_left[0] + pieces_left[1]) > 0)
  {
    if (command_counter > 1)
    {
      printf("\n[ERROR] You must first place all your remaining pieces.\n\n");
      return 0;
    }
    return placingPhase(user_input, size, board, player, pieces_left);
  }

  if ((pieces_left[0] + pieces_left[1]) <= 0)
  {
    if(command_counter == 1)
    {
      printf("\n[ERROR] You must specify a piece to shift and a target field.\n\n");
      return 0;
    }
    return movingPhase(user_input, size, board, player);
  }
  return -1;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if any of winning patterns is reachec
/// @param size The dimension the board
/// @param board The game board array
/// @param player Current player
/// @return true if one matches, otherwise false
bool checkWin(int size, int board[size][size], int player)
{
  for (int row = 0; row < size; row++)
  {
    for (int column = 0; column < size; column++)
    {
      if (board[row][column] != player)
      {
        continue;
      }

      if (column <= size - WINNING_PIECES)
      {
        if (board[row][column+1] == player && board[row][column+2] == player && board[row][column+3] == player)
        {
          return true;
        }
      }

      if (row <= size - WINNING_PIECES)
      {
        if (board[row+1][column] == player && board[row+2][column] == player && board[row+3][column] == player)
        {
          return true;
        }
      }

      if (row < size - 1 && column < size - 1)
      {
        if (board[row][column+1] == player && board[row+1][column] == player && board[row+1][column+1] == player)
        {
          return true;
        }
      }

      if (row <= size - WINNING_PIECES && column <= size - WINNING_PIECES)
      {
        if (board[row+1][column+1] == player && board[row+2][column+2] == player && board[row+3][column+3] == player)
        {
          return true;
        }
      }

      if (row >= 3 && column <= size - WINNING_PIECES)
      {
        if (board[row-1][column+1] == player && board[row-2][column+2] == player && board[row-3][column+3] == player)
        {
          return true;
        }
      }
    }
  }
  return false;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Determines whos turn it is. Contains main loop that handles game loop and ends game if someone winds
/// @param size The dimension the board
/// @param board The game board array
/// @param color_status Passes on if game is colored or not
/// @param player_1 ID for player 1
/// @param player_2 ID for player 2
/// @return Returns exit code
int initializeGame(int size, int board[size][size], bool color_status, int player_1, int player_2)
{
  int current_player = 0;
  int player_current_pieces[] = {PIECES_TO_PLACE,PIECES_TO_PLACE};
  int move_counter = 0;
  int display_round = 0;
  int status = 1;

  while(1)
  {  
    current_player = (move_counter % 2) + 1;
    display_round = (move_counter / 2) + 1;
    if (status == 1)
    {
      printf("\n===| Move %d: Player %d |===\n\n", display_round, current_player);

      printBoard(size, board, color_status, player_1, player_2);
      if ((player_current_pieces[0] + player_current_pieces[1]) > 0)
      {
        printRemainingPieces(player_current_pieces[0], player_current_pieces[1]);
      }
    }
  
    status = checkBoard(size, board, current_player, player_current_pieces);
    
    if (status == 0)
    {
      continue;
    }
    else if (status == -1)
    {
      break;
    }

    if (checkWin(size, board, current_player))
    {
      printf("\n===| Final Game State |===\n\n");
      printBoard(size, board, color_status, player_1, player_2);
      
      printf("===| Winner: Player %d |===\n", current_player);
      
      return current_player;
    }

    status = 1; 
    move_counter++;
  }
  return 0;
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Checks if color mode is "enabled" or "disabled" (case-insensitive)
/// @param argv The string argument to check
/// @return 1 if the string is "enabled", 0 if "disabled", or -1 if it matches neither
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

  if (strcmp(lowercase_copy, "enabled") == 0)
  {
    return 1;
  }
  else if ((strcmp(lowercase_copy, "disabled") == 0))
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Validates the number and content of command line arguments
/// @param argc The count of command line arguments
/// @param argv The array of command line argument strings
/// @return 1 if color output is enabled, 0 if disabled (default), or -1 on error
int returnCommandLineArgument(int argc, char* argv[])
{
  if (argc > 2)
  {
    printf("\n[ERROR] Usage: ./a2 [optional: color_enabled]\n\n");
    return -1;
  }

  int mode = 0;
  if (argc == 1)
  {
    mode = 0;
  }
  else 
  {
    mode = checkCommandLineArgument(argv[1]);
  }
  
  if (mode == 1)
  {
    return 1;
  }
  else if (mode == 0)
  {
    return 0;
  }
  else 
  {
    printf("\n[ERROR] (Optional) color output argument can either be \"disabled\" or \"enabled\".\n\n");
    return -1;
  }
}

//----------------------------------------------------------------------------------------------------------------------
/// @brief Handles argument validation, board initialization, and starts the main game loop
/// @param argc The number of command line arguments
/// @param argv The array of command line argument strings
/// @return The exit code: the winning player ID (1 or 2), 0 for a valid game end, or -1 on error
int main(int argc, char* argv[])
{
  int state = returnCommandLineArgument(argc, argv);
  if (state == -1)
  {
    return -1;
  }

  int board[BOARD_SIZE][BOARD_SIZE] = {0};
  printWelcome();
  return initializeGame(BOARD_SIZE, board, state, PLAYER_ONE_ID, PLAYER_TWO_ID);
}
