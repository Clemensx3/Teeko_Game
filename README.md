# Assignment 2 - Intricate Square Placement

## Introduction

In Assignment 2 (A2) of the ISP practicals (KU), you will implement a program that is inspired by the strategy game
[Teeko](https://boardgamegeek.com/boardgame/9138/teeko).

You do not need prior knowledge of the game to complete the assignment, although understanding it may help. Online versions are available if you want to try it. Please make sure that you follow the assignment description exactly, because even slight differences can cause testcases to fail.

This document describes the structure of the assignment and the expected functionality of the program.

## Learning Goals

Pointers, Strings, Arrays, Command Line Arguments, Constants

## Specifications

There are some general specifications that must be followed for every assignment in this course. A list of these and which point deductions may result from violating them can be found here: [Assessment of Assignments - Deductions](https://www.notion.so/coding-tugraz/Assessment-of-Assignments-How-do-you-get-your-points-d7cca6cc89a344e38ad74dd3ccb73cb3?pvs=4#e3203e50b1fc4ba787bdbf5fb46262ae)

> **Note:** You may use any library that is part of the C17 standard library. A complete list of C standard library headers can be found [here](https://en.cppreference.com/w/c/header.html).

## Grading

Assignment 2 is worth **18 points**.

Information on how your assignment submission will be assessed can be found here: [Assessment of Assignments](https://www.notion.so/coding-tugraz/Assessment-of-Assignments-How-do-you-get-your-points-d7cca6cc89a344e38ad74dd3ccb73cb3).
Please read this carefully, as it also contains guidelines on what your final code should look like (Coding Standard, Style Guide).

Here you can find an overview of how your total grade for the KU will be calculated: [Procedure and Grading](https://www.notion.so/coding-tugraz/Procedure-and-Grading-59975fee4a9c4047867772cd20caf73d)

## Submission

- Deliverables: `a2.c`
- Push to your repository
- **Deadline: 19.11.2025, 18:00 (Austrian time)**

## Weekly Exercises

There are **Weekly Exercises** associated with this assignment available on [TeachCenter](https://tc.tugraz.at/main/course/section.php?id=72261). These are simple exercises designed to help you prepare for the assignment, and **must** be solved at least partially to avoid deductions.

> **Attention**: At least 50% of the Weekly Exercises for A2 must be solved before the A2 deadline. Failure to meet this requirement will result in a **25% point deduction on A2**.

## Assignment Description

The goal of the game is to arrange all four of your pieces on the game board either in a straight line (vertical, horizontal, or diagonal) or in a 2 &times; 2 square.

### General Notes

The notes given in this section are important for the entire assignment description:

- `\n` should not be printed as separate characters, instead they represent the newline character.
- Note the leading and trailing spaces in the text fields! You can make them visible by selecting the text field.
- Words in angle brackets (such as `<PIECE>`) should be replaced by calculated or given data. They are **not** part of the output itself! For a better understanding, you can look at the [example outputs](#example-outputs) at the very bottom of this document.
- The user input is **case-insensitive**. This means that `QUIT`, `quit` and `qUiT` should be treated equally.
- <code>P1&nbsp;>&nbsp;</code> or <code>P2&nbsp;>&nbsp;</code> (command prompt, always printed with a trailing space) in the output indicates that the program should wait for user input at this point. Execution should only continue after the user submits their input with `Enter`.
- If the user's input is invalid, the corresponding error message should be printed. After this, the same question is asked again, the command prompt is printed, and the user can input again. This process is repeated **as long as** the input is invalid.
- All leading and trailing whitespaces in the input, as well as any extra spaces between parts of the input, should be ignored.
- Make sure that your program can handle inputs of arbitrary length including empty inputs. It is important that your program does not crash, even if the user input is longer than what you want to store. In such cases, store only what is necessary and discard the rest.
- Whenever the program waits for user input, it must correctly handle `EOF` (end of file, **_not_** the string "EOF"). Receiving `EOF` should terminate the program immediately with a return value of `0`. `EOF` can be entered manually with the keyboard using `Ctrl+D`. Use this to test whether your program behaves correctly.

### Framework

To reduce your workload, we have created a small framework that helps with printing the game board. It is available in the files `framework.c` and `framework.h` in your repository.

You **do not have to** use this framework, it is completely optional. If you decide **not** to use it, **you will need to implement the board printing functionality yourself** (see Program Sequence > Playing the Game > Printing the Board).

If you use the provided framework, it expects the board to be passed as a two-dimensional array, where the first index represents the row and the second index represents the column. While you are not required to store your board in this way, doing so is recommended to ensure compatibility with the framework’s printing function.

The framework includes one function that prints the board for you (see the header file `framework.h` for more details):

```c
void printBoard(int size, int board[size][size], bool color_status, int player_1, int player_2);
```

> **Note:**
> - A **.h** file (header file) contains function declarations and documentation explaining how to use them.
> - A **.c** file contains the actual implementations of those functions.
When you include the header file in your program (by adding the line `#include "framework.h"` at the top of your `a2.c` file), you can call the function implemented in `framework.c`.

> **Attention:** The **original, unmodified versions** of the files `framework.c` and `framework.h` will be used when testing your program. **Therefore, do not make any changes to these files, as any modifications you make will be lost in the test system.**

## Program Sequence

The program runs interactively, allowing two players to alternate turns. It updates the board after each move and checks for a winner. The game stops immediately when a player wins or if the game is quit.

<details>
<summary><h3>Starting the Game</h3></summary>

The program may be started with a single command-line argument, which controls whether color mode is enabled. Afterwards, the players are greeted with a welcome message.

<details>
<summary><h4>Parsing Command Line Arguments</h4></summary>

The program can be started with one optional argument that specifies whether color output is enabled.

Syntax: `./a2 [COLOR_ENABLED]`

- `[COLOR_ENABLED]` must either be omitted or case-insensitively match `disabled` or `enabled`.

If the number of command line arguments is not valid, or any of the above rules are not followed, the program should print the correct error message as listed in the [Return Values and Error Messages](#return-values-and-error-messages)
section and terminate with the correct return value.

If the optional argument `[COLOR_ENABLED]` is omitted, a default value of `disabled` should be used for the game.

**Examples:**

Valid program starts: `./a2`, `./a2 disabled`, `./a2 ENabled` \
Invalid program starts: `./a2 42`, `./a2 off`, `./a2 disabled enabled`

</details>

<details>
<summary><h4>Welcome Message</h4></summary>

Next, the players are welcomed with the following message:

```
===| Welcome to       |===\n
===| [I]ntricate      |===\n
===| [S]quare         |===\n
===| [P]lacement      |===\n
\n
```

</details>

</details>

<details>
<summary><h3>Playing the Game</h3></summary>

The players take turns, first placing all their pieces on the board, and then moving them one step at a time.

<details>
<summary><h4>Moves</h4></summary>

Each player makes one move per turn, starting with Player 1. A counter shall be implemented to display the number of moves in the game. There are two types of moves in the game: **placing pieces** and **shifting pieces**.

Before each move, the current move number and the active player should be printed as follows:
```
\n
===| Move <MOVE_NUMBER>: Player <PLAYER_ID> |===\n
\n
```
`<MOVE_NUMBER>` corresponds to the current move _per player_, meaning that the first move of Player 1 and the first move of Player 2 will both be numbered `1`. `<PLAYER_ID>` shall be replaced by `1` or `2`, depending on which player is currently taking their turn.

Next, the board shall be printed as described in Program Sequence > Playing the Game > Printing
the Board.

Following this, **if there are still pieces left to be placed on the board**, the remaining pieces shall be printed as described in Program Sequence > Playing the Game > Printing Remaining Pieces.

Finally, the command prompt for the current player shall be printed as follows (mind the trailing space):
```
P<PLAYER_ID> >
```

`<PLAYER_ID>` shall be replaced by `1` or `2`, depending on which player is currently taking their turn.

Please refer to [Example Outputs](#example-outputs) for more details.

#### Placing Pieces Phase

The first **four moves** of each player must be used to place their pieces on any empty field (i.e., non-occupied position) on the board.

To do this, the player must enter the coordinate of the desired field in the format
`<COLUMN_LABEL><ROW_LABEL>`, where `<COLUMN_LABEL>` and `<ROW_LABEL>` correspond to the labels printed next to the board, as shown in Program Sequence > Playing the Game > Printing the Board.

Any **case-insensitive** combination of a single letter followed by a single digit is considered a _valid_ coordinate format. However, the referenced field may still be **invalid**, **out of bounds**, or **already occupied**. For more details, see [Return Values and Error Messages](#return-values-and-error-messages).

> **Hint:** The function `getchar()` can be useful for reading input character by character, allowing you to store or discard characters as needed.

**Example:** Player 1 tries to place a piece in the **first column** (column **A**) of the **second row** (row **2**):
```
P1 > a2
```

> **Note:** Naturally, the number of remaining pieces will decrease if the piece was successfully placed on the board.

If, during this piece-placing phase, a player enters input that would only be valid _after_ all pieces have been placed on the board (i.e., during the piece-shifting phase), it shall be treated as an error (see [Return Values and Error Messages](#return-values-and-error-messages)).

#### Shifting Pieces Phase

After all eight pieces (four per player) have been placed on the board, the game enters the piece-shifting phase. In this phase, both players take turns moving one of their pieces one field at a time.

A single coordinate (which was _valid_ input during the piece-placing phase) is now considered _invalid_ input and shall be treated as an error (see [Return Values and Error Messages](#return-values-and-error-messages)). During the piece-shifting phase, _valid_ input must follow the format `<COLUMN_LABEL><ROW_LABEL> <COLUMN_LABEL><ROW_LABEL>`.

The first coordinate specifies the location of the piece to be moved, and the second coordinate specifies the target field to move it to.

> **Attention:** The two coordinates can be separated by any amount of whitespace, but there must be **at least one** whitespace character between them.

A shifting move is considered valid if all of the following conditions are met:
- The first coordinate targets a field containing a **piece of the current player**.
- The second coordinate targets a field that is **empty** and **adjacent** to the piece being moved. _Adjacent_ means the empty field is **directly next to** the piece's current field, either **vertically**, **horizontally**, or **diagonally**.

If any of these conditions are not met, the corresponding error message listed in [Return Values and Error Messages](#return-values-and-error-messages) shall be printed.

</details>

<details>
<summary><h4>Printing the Board</h4></summary>

> **Note:** A framework for printing the board is provided, see [Framework](#framework) for details. This section is **only relevant** if you choose **not** to use the provided framework and implement the board printing yourself.
If you use the framework, you can **safely ignore** this section.

First, add the following `define` statements at the beginning of your source file:
```c
#define UNICODE_MIDDLE_DOT "\u00b7"
#define UNICODE_SQUARE_FILLED "\u25a0"
#define UNICODE_SQUARE_EMPTY "\u25a1"
```

> **Note:** You are encouraged to redefine these constants with names that are more meaningful for your program. For example:
>```c
>#define PIECE_PLAYER_1 UNICODE_SQUARE_FILLED
>```

The board shall be displayed as a square 5 &times; 5 grid. It is surrounded by row and column labels to help players identify board positions.

**Empty board example:**

```
     A  B  C  D  E\n
   + -------------\n
 1 | ·  ·  ·  ·  ·\n
 2 | ·  ·  ·  ·  ·\n
 3 | ·  ·  ·  ·  ·\n
 4 | ·  ·  ·  ·  ·\n
 5 | ·  ·  ·  ·  ·\n
\n
```

**Steps for printing the board:**
1. Print the line <code>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A&nbsp;&nbsp;B&nbsp;&nbsp;C&nbsp;&nbsp;D&nbsp;&nbsp;E\n</code>
2. Print the separator line <code>&nbsp;&nbsp;&nbsp;+ -------------\n</code>
3. For each row of the board, print the line <code>&nbsp;<ROW_LABEL> | &lt;FIELD&gt;  &lt;FIELD&gt;  &lt;FIELD&gt;  &lt;FIELD&gt;  &lt;FIELD&gt;\n</code>
4. Print an additional empty line `\n`.

`<ROW_LABEL>` shall be a single-digit number starting at `1` for the topmost row, and ending at `5` for the bottommost row.

If a field on the board is empty (i.e., no piece occupies it), `<FIELD>` shall be replaced by the defined `UNICODE_MIDDLE_DOT`. If a field is occupied by a player's piece, `<FIELD>` shall be replaced by `<PIECE>`, as described in the following sections on output with and without color enabled.

The program must take into account whether color output was enabled through the command line argument at program start.
Depending on this setting, the board output shall differ accordingly.

#### Without Color

`<PIECE>` shall be replaced by the defined `UNICODE_SQUARE_FILLED` for Player 1 and by `UNICODE_SQUARE_EMPTY` for Player 2.

#### With Color

Most modern command-line interfaces support colored output by sending special character sequences to the standard output.
These sequences are known as [ANSI escape sequences](https://en.wikipedia.org/wiki/ANSI_escape_code).

First, add the following `define` statements at the beginning of your source file:
```c
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_RED "\x1b[91m"
#define ANSI_COLOR_YELLOW "\x1b[93m"
```

> **Note:** You are encouraged to redefine these constants with names that are more meaningful for your program. For example:
>```c
>#define COLOR_PLAYER_1 ANSI_COLOR_RED
>```

<details>
<summary><b>Explanation of ANSI Escape Sequences</b></summary>

- `\x1b` is the ANSI escape character, starting a command/escape sequence
- `[` is the ANSI control sequence introducer, selecting a specific range of commands
- `0` is the argument to reset all attributes, including the color
- `91` is the argument for the color "Bright Red"
- `93` is the argument for the color "Bright Yellow"
- `m` signals the end of the command sequence, executing the command

</details>

<br/>

When printing the board, you should use these constants to display the pieces in different colors, if color output was enabled at the start of the program.

`<PIECE>` shall now _always_ be replaced by `UNICODE_SQUARE_FILLED`, and printed in `ANSI_COLOR_RED` for Player 1 or `ANSI_COLOR_YELLOW` for Player 2.

**Example:** Printing a red piece (Player 1):
```c
printf(ANSI_COLOR_RED UNICODE_SQUARE_FILLED ANSI_COLOR_RESET);
```

</details>

<details>
<summary><h4>Printing Remaining Pieces</h4></summary>

As long as all eight pieces have **not yet been placed** on the board (piece-placing phase), the following shall be printed below the board:

```
---| Remaining Pieces |---\n
\n
 Player 1:  <REMAINING_PIECES_1>\n
 Player 2:  <REMAINING_PIECES_2>\n
\n
```

`<REMAINING_PIECES_1>` and `<REMAINING_PIECES_2>` shall be replaced with the number of pieces that Player 1 and Player 2 still need to place on the board, respectively.

</details>

</details>

<details>
<summary><h3>Ending the Game</h3></summary>

The program ends either when the user stops the game or when a player wins.

<details>
<summary><h4>Command <code>quit</code> or <code>EOF</code></h4></summary>

The program shall terminate immediately with a return value of `0` if:
- the `quit` command is entered by either player at any point during the game, or
- `EOF` (end of file, **_not_** the string "EOF") is received instead of a regular user input (e.g., via `Ctrl+D`).

</details>

<details>
<summary><h4>Player Wins the Game</h4></summary>

The game is won when a **winning pattern** is achieved by either player, which can occur during either the piece-placing or piece-shifting phase. After placing or shifting a piece that forms a winning pattern, the following message is printed:

```
\n
===| Final Game State |===
\n
```

Then, the final **state of the board should be printed** in the usual format. (see Program Sequence > Playing the Game > Printing the Board)
Afterwards, print the following message:

```
===| Winner: Player <PLAYER_ID> |===\n
```

The program shall terminate with a return value of `<PLAYER_ID>`, where `<PLAYER_ID>` is either `1` or `2`, depending on which player created the winning pattern on the board.

The following are considered **winning patterns**:
```
·  ·  ·  · | ·  ·  ■  · | ■  ·  ·  · | ·  ·  ·  ■ | ·  ·  ·  ·
■  ■  ■  ■ | ·  ·  ■  · | ·  ■  ·  · | ·  ·  ■  · | ·  ■  ■  ·
·  ·  ·  · | ·  ·  ■  · | ·  ·  ■  · | ·  ■  ·  · | ·  ■  ■  ·
·  ·  ·  · | ·  ·  ■  · | ·  ·  ·  ■ | ■  ·  ·  · | ·  ·  ·  ·
```

> **Note:** For demonstration purposes, all boards are shown next to each other; this is not valid program output. Additionally, opponent pieces are not displayed on these boards. These patterns can appear anywhere on the board where they fit within its bounds.

</details>

</details>

## Return Values and Error Messages

The program has to react to errors during the game and print the corresponding error messages and/or return the correct return values. You can find all error messages that should be printed and all return values in the table below.

Only one error should be printed at once. If multiple errors occur at the same time, only the highest message in the below table should be printed.

> **Attention:** An additional empty line `\n` shall be printed **before** and **after** each error message in the table below.

> **Note:** After an error that does not terminate the program (i.e., no return value is specified in the table below), the input message and command prompt shall be printed again, as described in [General Notes](#general-notes).

| Return Value | Error Message                                                                       | Meaning                                                                                                                    |
|--------------|-------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------|
| `0`          | &mdash;                                                                             | The program terminated because of the `quit` command or `EOF`.                                                             |
| `1`          | &mdash;                                                                             | The program terminated because Player 1 won the game.                                                                      |
| `2`          | &mdash;                                                                             | The program terminated because Player 2 won the game.                                                                      |
| `-1`         | `[ERROR] Usage: ./a2 [optional: color_enabled]\n`                                   | Invalid amount of command line arguments at program start.                                                                 |
| `-1`         | `[ERROR] (Optional) color output argument can either be "disabled" or "enabled".\n` | The optional color output argument at program start was given and is not `disabled` or `enabled`.                          |
| &mdash;      | `[ERROR] Unknown command.\n`                                                        | The entered command is not a known command of the program (valid commands: `<COORDINATE>`, `<COORDINATE> <COORDINATE>`, `quit`).                                                                 |
| &mdash;      | `[ERROR] You must first place all your remaining pieces.\n`                         | [Placing] A player attempts to shift one of their pieces before all of their pieces have been placed on the board.         |
| &mdash;      | `[ERROR] You must specify a piece to shift and a target field.\n`                   | [Shifting] A player attempts to place a piece even though they have no remaining pieces left to place.                     |
| &mdash;      | `[ERROR] Coordinates not within board.\n`                                           | [Both] Any of the entered coordinates target a field outside of the board's bounds.                                        |
| &mdash;      | `[ERROR] No shiftable piece at coordinates.\n`                                      | [Shifting] The first entered coordinate does not target a shiftable piece of the current player.                           |
| &mdash;      | `[ERROR] Field at coordinates occupied.\n`                                          | [Both] The entered coordinate (or the second coordinate when shifting) does not target an empty field.                     |
| &mdash;      | `[ERROR] Field at coordinates not adjacent to chosen piece.\n`                      | [Shifting] The second entered coordinate does not target a field adjacent to the chosen piece.                             |

## Example Outputs

<details>
<summary><h3>Exiting With 'quit'</h3></summary>

Program started with `./a2`.

```
===| Welcome to       |===
===| [I]ntricate      |===
===| [S]quare         |===
===| [P]lacement      |===


===| Move 1: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ·  ·  ·  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  ·  ·
 5 | ·  ·  ·  ·  ·

---| Remaining Pieces |---

Player 1: 4
Player 2: 4

P1 > b2

===| Move 1: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ·  ■  ·  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  ·  ·
 5 | ·  ·  ·  ·  ·

---| Remaining Pieces |---

Player 1: 3
Player 2: 4

P2 > D4

===| Move 2: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ·  ■  ·  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  □  ·
 5 | ·  ·  ·  ·  ·

---| Remaining Pieces |---

Player 1: 3
Player 2: 3

P1 > qUiT
```

Program ended by typing `quit`.
</details>

<details>
<summary><h3>Standard Gameplay Loop</h3></summary>

Program started with `./a2 disabled`.

```
===| Welcome to       |===
===| [I]ntricate      |===
===| [S]quare         |===
===| [P]lacement      |===


===| Move 1: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ·  ·  ·  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  ·  ·
 5 | ·  ·  ·  ·  ·

---| Remaining Pieces |---

Player 1: 4
Player 2: 4

P1 > b1

===| Move 1: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ·  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  ·  ·
 5 | ·  ·  ·  ·  ·

---| Remaining Pieces |---

Player 1: 3
Player 2: 4

P2 > d5

===| Move 2: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ·  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  ·  ·
 5 | ·  ·  ·  □  ·

---| Remaining Pieces |---

Player 1: 3
Player 2: 3

P1 > C2

===| Move 2: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ■  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  ·  ·
 5 | ·  ·  ·  □  ·

---| Remaining Pieces |---

Player 1: 2
Player 2: 3

P2 > D4

===| Move 3: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ■  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  ·  ·  □  ·
 5 | ·  ·  ·  □  ·

---| Remaining Pieces |---

Player 1: 2
Player 2: 2

P1 > b3

===| Move 3: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ■  ·  ·
 3 | ·  ■  ·  ·  ·
 4 | ·  ·  ·  □  ·
 5 | ·  ·  ·  □  ·

---| Remaining Pieces |---

Player 1: 1
Player 2: 2

P2 > B4

===| Move 4: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ■  ·  ·
 3 | ·  ■  ·  ·  ·
 4 | ·  □  ·  □  ·
 5 | ·  ·  ·  □  ·

---| Remaining Pieces |---

Player 1: 1
Player 2: 1

P1 > C4

===| Move 4: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ■  ·  ·
 3 | ·  ■  ·  ·  ·
 4 | ·  □  ■  □  ·
 5 | ·  ·  ·  □  ·

---| Remaining Pieces |---

Player 1: 0
Player 2: 1

P2 > c5

===| Move 5: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ■  ·  ·  ·
 2 | ·  ·  ■  ·  ·
 3 | ·  ■  ·  ·  ·
 4 | ·  □  ■  □  ·
 5 | ·  ·  □  □  ·

P1 > b1 B2

===| Move 5: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ·  ■  ■  ·  ·
 3 | ·  ■  ·  ·  ·
 4 | ·  □  ■  □  ·
 5 | ·  ·  □  □  ·

P2 > d4 C3

===| Move 6: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ·  ■  ■  ·  ·
 3 | ·  ■  □  ·  ·
 4 | ·  □  ■  ·  ·
 5 | ·  ·  □  □  ·

P1 > B3 a2

===| Move 6: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ■  ■  ■  ·  ·
 3 | ·  ·  □  ·  ·
 4 | ·  □  ■  ·  ·
 5 | ·  ·  □  □  ·

P2 > c3 d4

===| Move 7: Player 1 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ■  ■  ■  ·  ·
 3 | ·  ·  ·  ·  ·
 4 | ·  □  ■  □  ·
 5 | ·  ·  □  □  ·

P1 > c4 d3

===| Move 7: Player 2 |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ■  ■  ■  ·  ·
 3 | ·  ·  ·  ■  ·
 4 | ·  □  ·  □  ·
 5 | ·  ·  □  □  ·

P2 > b4 c4

===| Final Game State |===

     A  B  C  D  E
   + -------------
 1 | ·  ·  ·  ·  ·
 2 | ■  ■  ■  ·  ·
 3 | ·  ·  ·  ■  ·
 4 | ·  ·  □  □  ·
 5 | ·  ·  □  □  ·

===| Winner: Player 2 |===
```

Program terminated with a return value of `2`.

</details>
