// NxN tic-tac-toes
#include <iostream>
#include <cstdlib>
using namespace std;

// The following 3 functions are helper functions to convert
// between 1D and 2D array indices.  The grid itself is stored
// as a 1D array.  However, for printing, checking who won,
// etc. it may be easier to write loops that generate separate
// row/column indices (i.e. treating the array like a 2D array).
// The functions below should be written using the hints in the
// writeup for performing the appropriate arithmetic conversion
// between 1D- and 2D-indices, and can then be called in any
// of the other functions when you find yourself wanting to
// convert one to the other.

/**
 * Helper function - Given the grid array and its dimension
 *    as well as a particular row (r) and column (c), this
 *    function performs the arithmetic to convert r and c
 *    to a 1D index and returns that character in the grid.
 *    For example, for dim = 3 and r = 2, c = 1, this function
 *    should compute the corresponding index: 7 and return
 *    grid[7].
 *
 * Use this function wherever you generate, row/column
 *    indices and want to access that element in the grid.
 *    Pass the row/column indices as r and c, respectively.
 *
 */
char getEntryAtRC(char grid[], int dim, int r, int c);

/**
 * Helper function - Given a 1D index returns the row
 * that corresponds to this 1D index.  Use this in
 * conjunction with idxToCol() anytime you have a 1D index
 * and want to convert to 2D indices.
 */
int idxToRow(int idx, int dim);

/**
 * Helper function - Given a 1D index returns the column
 * that corresponds to this 1D index.  Use this in
 * conjunction with idxToRow() anytime you have a 1D index
 * and want to convert to 2D indices.
 */
int idxToCol(int idx, int dim);


/** Should print the tic-tac-toe board in a nice grid
 *  format as shown below:
 *
 * Parameters:
 *   grid: Array of dim^2 characters representing the state
 *         of each square.  Each entry contains 'X', 'O'
 *         (the letter oh), or '?'.
 */
void printTTT(char grid[], int dim);

/**
 * Should check if any player has won the game yet.
 *
 * Parameters:
 *   grid: Array of dim^2 characters representing the state
 *         of each square.  Each entry contains 'X', 'O', or '?'
 *
 * Return value:
 *   Should return 1 if 'X' has won, 2 if 'O' has won, or 0 (zero)
 *   if the game is still undecided.
 *
 */
int checkForWinner(char grid[], int dim);

/**
 * Should check if there is no possible way any player can win.
 * More specifically, if there does not exist a row, column,
 * or diagonal that can still have 3 of a kind, then the game
 * will be a draw.
 *
 *
 * Parameters:
 *   grid: Array of dim^2 characters representing the state
 *         of each square.  Each entry contains 'X', 'O', or '?'
 *
 * Return value:
 *   Return true if no player can win given the current
 *   game state, false otherwise.
 */
bool checkForDraw(char grid[], int dim);

/**
 * @brief Get the Ai Choice for the current player and update grid object
 *
 * Parameters:
 *   grid: Array of dim^2 characters representing the state
 *         of each square.  Each entry contains 'X', 'O', or '?'
 *   dim: the dim(-ension) of the tic-tac-toe board
 *   player: current player => 'X' or 'O'
 * @return true If an error occurred or a choice was unable to be made
 * @return false If a play was successfully made
 */
bool getAiChoiceAndUpdateGrid(char grid[], int dim, char player);

/**
 * @brief Picks a random location for the current player and update grid
 *
 * Parameters:
 *   grid: Array of dim^2 characters representing the state
 *         of each square.  Each entry contains 'X', 'O', or '?'
 *   dim: the dim(-ension) of the tic-tac-toe board
 *   player: current player => 'X' or 'O'
 * @return true If no viable location to be played
 * @return false If a play was successfully made
 */
bool getRandChoiceAndUpdateGrid(char grid[], int dim, char player);

/**********************************************************
 *  Write your implementations for each function prototyped
 *  above in the space below
 **********************************************************/

char getEntryAtRC(char grid[], int dim, int r, int c)
{
  return grid[dim*r+c]; // dimension * row + column returns grid char val
}

int idxToRow(int idx, int dim)
{
  return idx/dim; // int division to get corresponding row val
}

int idxToCol(int idx, int dim)
{
  return idx%dim; // return remainder for corresponding col val
}

void printTTT(char grid[], int dim) 
{
  for (int i = 0; i < dim; i++) // for dim num of rows
  {
    for (int j = 0; j < dim; j++) // for dim num of cols
    {
      cout << " " << getEntryAtRC(grid, dim, i, j) << " "; // print array with spaces
      if (j < dim - 1) // don't print vertical separator after last val in row
      { 
        cout << "|"; // col separators
      }
    }
    cout << endl; // horizontal separators start on newline
    for (int k = 0; k < dim; k++) // for dim num of horizontal separators
    {
      // don't print horizontal separator under last row
      if (i < dim - 1 && k != dim - 1) // three dashes for each index and one dash for vertical separator 
      {
        cout << "----";
      }
      if (i < dim - 1 && k == dim - 1) // only print three dashes under last val of row because no vertical separator after
      {
        cout << "---"; 
      }
    }
    if (i < dim - 1)
    {
      cout << endl; // newline after each row
    }
  }
}

int checkForWinner(char grid[], int dim)
{

  // tracker variable declaration
  int X_count_row;
  int O_count_row;
  int X_count_col;
  int O_count_col;
  int X_count_diag_one;
  int O_count_diag_one;
  int X_count_diag_two;
  int O_count_diag_two;
  
  // check for winner in row
  for (int i = 0; i < dim*dim; i+=dim) 
  {
    // reinitialize on each row
    X_count_row = 0;
    O_count_row = 0;
    for (int j = dim*i; j < dim*(i+1); j++) 
    {
      if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == 'X')
      {
        X_count_row++;
        if (X_count_row == dim) 
        {
          return 1;
        }
      }
      else if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == 'O')
      {
        O_count_row++;
        if (O_count_row == dim)
        {
          return 2;
        }
      }
    }
  }
  // if no winner in row
  if (X_count_row != dim || O_count_row != dim) 
  {
    // check each col
    for (int j = 0; j < dim; j++)
    {
      // reinitialize on each col
      X_count_col = 0;
      O_count_col = 0;
      for (int i = 0; i < dim*dim; i+=dim)
      {
        if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == 'X')
        {
          X_count_col++;
          if (X_count_col == dim)
          {
            return 1; // 'X' wins
          }
        }
        else if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == 'O')
        {
          O_count_col++;
          if (O_count_col == dim)
          {
            return 2; // 'O' wins
          }
        }
      }
    }
  }
  // if no winner in col
  if (X_count_col != dim && O_count_col != dim) 
  {
    // check first diag
    X_count_diag_one = 0;
    O_count_diag_one = 0;
    for (int i = 0; i < dim*dim; i+=dim+1)
    {
      if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == 'X')
      {
        X_count_diag_one++;
        if (X_count_diag_one == dim)
        {
          return 1; // 'X' wins on first diag
        }
      }
      else if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == 'O')
      {
        O_count_diag_one++;
        if (O_count_diag_one == dim)
        {
          return 2; // 'O' wins
        }
      }
    }
  }
  // check second diag
  if (X_count_diag_one != dim || O_count_diag_one != dim)
  { 
    X_count_diag_two = 0;
    O_count_diag_two = 0;
    for (int i = dim-1; i < ((dim*dim)-dim)+1; i+=dim-1) 
    {
      if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == 'X')
      {
        X_count_diag_two++;
        if (X_count_diag_two == dim)
        {
          return 1; 
        }
      }
      else if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == 'O')
      {
        O_count_diag_two++;
        if (O_count_diag_two == dim)
        {
          return 2; // 'O' wins
        }
      }
    }
  }
  return 0;
}

bool checkForDraw(char grid[], int dim)
{

  // tracker variable declaration
  int X_count_row;
  int O_count_row;
  int row_draw = 0;
  int X_count_col;
  int O_count_col;
  int col_draw = 0;
  int X_count_diag_one;
  int O_count_diag_one;
  int diag_one_draw;
  int X_count_diag_two;
  int O_count_diag_two;
  int diag_two_draw;

  // check draw in row
  for (int i = 0; i < dim*dim; i+=dim) 
  {
    X_count_row = 0;
    O_count_row = 0;
    for (int j = dim*i; j < dim*(i+1); j++) 
    {
      if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == 'X')
      {
        X_count_row++; // one 'X' in row
      }
      else if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == 'O')
      {
        O_count_row++; // one 'O' in row
      }
      if (X_count_row >= 1 && O_count_row >= 1) 
      {
        row_draw++;
        break;
      }
    }
  }
  // if no draw in row, game is not over 
  if (row_draw == 0)
  {
    return false;
  }
  // continue checking if draw in row
  if (row_draw >= 1) 
  { 
    for (int j = 0; j < dim; j++)
    {
      X_count_col = 0;
      O_count_col = 0;
      for (int i = 0; i < dim*dim; i+=dim) // check col
      {
        if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == 'X')
        {
          X_count_col++;
        }
        else if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == 'O')
        {
          O_count_col++;
        }
      }
      if (X_count_col >= 1 && O_count_col >= 1)
      {
        col_draw++;
        break;
      }
    }
  }
  // if col draw val is the same as num of cols
  if (col_draw == dim)
  {
    return true;
  }
  if (col_draw == 0)
  {
    return false;
  }
  // if no draw in every row or col, game is not over yet, check first diag
  if (row_draw >= 1 && row_draw < dim && col_draw >= 1 && col_draw < dim) 
  {
    X_count_diag_one = 0;
    O_count_diag_one = 0;
    diag_one_draw = 0;
    for (int i = 0; i < dim*dim; i+=dim+1)
    {
      if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == 'X')
      {
        X_count_diag_one++;
      }
      else if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == 'O')
      {
        O_count_diag_one++;
      }
      if (X_count_diag_one >= 1 && O_count_diag_one >= 1)
      {
        diag_one_draw++;
        break;
      }
    }
  }
  if (diag_one_draw == 0) 
  {
    return false;
  }
  // check second diag if at least one draw on row, col, and diag
  if (row_draw >= 1 && col_draw >= 1 && diag_one_draw >= 1)
  {
    X_count_diag_two = 0;
    O_count_diag_two = 0;
    diag_two_draw = 0;
    for (int i = dim-1; i < ((dim*dim)-dim)+1; i+=dim-1) 
    {
      if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == 'X')
      {
        X_count_diag_two++;
      }
      else if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == 'O')
      {
        O_count_diag_two++;
      }
      if (X_count_diag_two >= 1 && O_count_diag_two >= 1)
      {
        diag_two_draw++;
      }
    }
  }
  if (diag_two_draw == 0) 
  {
    return false;
  }
  // at least one 'X' and one 'O' on each row, each col, diag one, and diag two
  if (row_draw == dim && col_draw == dim && diag_one_draw >= 1 && diag_two_draw >= 1)
  { 
    return true; // draw
  }
  else 
  {
    return false; // not a draw
  }
}

bool getAiChoiceAndUpdateGrid(char grid[], int dim, char player)
{
  // variable declaration
  int new_player_input;
  int row_move;
  int col_move;
  int diag_one_move;
  int diag_two_move;
  int opponent; // track other player

  // win on row?
  for (int i = 0; i < dim*dim; i+=dim)
  {
    // initialize on each row
    row_move = 0;
    opponent = 0;
    for (int j = dim*i; j < dim*(i+1); j++)
    {
      if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == player)
      {
        row_move++;
      }
      // not curr player and not '?', must be opponent
      if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) != player && getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) != '?')
      { 
        opponent++;
      }
      // block
      if (opponent == dim - 1) 
      {
        // reset new player input to update array
        new_player_input = dim*idxToRow(i, dim)+idxToCol(j, dim)+1;
        grid[new_player_input] = player;
        return false;
      }
      // auto win
      if (j == dim*(i+1) - 1 && row_move == dim - 1 && getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == '?')
      {
        new_player_input = dim*idxToRow(i, dim)+idxToCol(j, dim);
        grid[new_player_input] = player;
        return false;
      }
    }
  }

  // win on col?
  for (int j = 0; j < dim; j++)
  {
    // var initialization
    col_move = 0;
    opponent = 0;
    for (int i = 0; i < dim*dim; i+=dim) 
    {
      if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == player)
      {
        col_move++;
      }
      if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) != player && getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) != '?')
      { 
        opponent++;
      }
      if (opponent == dim - 1 && getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == '?')
      {
        new_player_input = dim*idxToRow(i, dim)+idxToCol(j, dim);
        cout << new_player_input << endl;
        grid[new_player_input] = player;
        return false;
      }
      if (i == dim*dim - 1 && col_move == dim - 1 && getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(j, dim)) == '?')
      {
        new_player_input = dim*idxToRow(i, dim)+idxToCol(j, dim);
        grid[new_player_input] = player;
        return false;
      }
    }
  }

  // win on first diag?
  for (int i = 0; i < dim*dim; i+=dim+1)
  {
    diag_one_move = 0;
    opponent = 0;
    if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == player)
    {
      diag_one_move++;
    }
    cout << getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) << endl;
    if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) != player && getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) != '?')
    { 
      opponent++;
    }
    if (opponent == dim - 1)
    {
      new_player_input = dim*idxToRow(i, dim)+idxToCol(i, dim)+dim+1;
      cout << new_player_input << endl;
      grid[new_player_input] = player;
      return false;
    }
    if (i == dim*dim - 1 && diag_one_move == dim - 1 && getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == '?')
    {
      new_player_input = dim*idxToRow(i, dim)+idxToCol(i, dim);
      grid[new_player_input] = player;
      return false;
    }
  }

  // win on second diag?
  for (int i = dim-1; i < dim*dim; i+=dim-1) 
  {
    diag_two_move = 0;
    opponent = 0;
    if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == player)
    {
      diag_two_move++;
    }
    if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == '?')
    {
      new_player_input = dim*idxToRow(i, dim)+idxToCol(i, dim);
      grid[new_player_input] = player;
      return false;
    }
    if (getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) != player && getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) != '?')
    { 
      opponent++;
    }
    if (opponent == dim - 1)
    {
      new_player_input = dim*idxToRow(i, dim)+idxToCol(i, dim)+dim-1;
      grid[new_player_input] = player;
      return false;
    }
    if (i == dim*dim - 1 && diag_two_move == dim - 1 && getEntryAtRC(grid, dim, idxToRow(i, dim), idxToCol(i, dim)) == '?')
    {
      new_player_input = dim*idxToRow(i, dim)+idxToCol(i, dim);
      grid[new_player_input] = player;
      return false;
    }
  }
  // else
  return true;
}

// Complete...Do not alter
bool getRandChoiceAndUpdateGrid(char grid[], int dim, char player)
{
  int start = rand()%(dim*dim);
  // look for an open location to play based on random starting location.
  // If that location is occupied, move on sequentially until wrapping and
  // trying all locations
  for(int i = 0; i < dim*dim; i++) 
  {
    int loc = (start + i) % (dim*dim);
    if (grid[loc] == '?') 
    {
      grid[loc] = player;
      return false;
    }
  }
  // No viable location
  return true;
}


/**********************************************************
 *  Complete the indicated parts of main(), below.
 **********************************************************/
int main()
{
  // This array holds the actual board/grid of X and Os. It is sized
  // for the biggest possible case (11x11), but you should only
  // use dim^2 entries (i.e. if dim=3, only use 9 entries: 0 to 8)
  char tttdata[121];

  // dim stands for dimension and is the side length of the
  // tic-tac-toe board i.e. dim x dim (3x3 or 5x5).
  int dim;
  int seed;

  // Start of game
  // Get the dimension from the user
  cin >> dim >> seed;
  srand(seed);

  int dim_sq = dim * dim;

  for(int i = 0; i < dim_sq; i++) 
  {
    tttdata[i] = '?';
  }

  // Print one of these messages when the game is over
  // and before you quit
  const char xwins_msg[] = "X player wins!";
  const char owins_msg[] = "O player wins!";
  const char draw_msg[] =  "Draw...game over!";

  bool done = false;
  char player = 'X'; // player is initialized as 'X' because first player will be 'X'
  
  // player input variable declared outside the loop
  int player_input;

  // Show the initial starting board
  printTTT(tttdata, dim);

  while (!done) // still playing
  {
    // prompt user each time in loop
    cout << "Player " << player << " enter your square choice [0-" << dim_sq-1 << "], -1 (AI), or -2 (Random): ";
    
    //**********************************************************
    // Get the current player's input (i.e. the location they want to
    // choose to place their mark [X or O]) or choice of AI or Random
    // location and update the tttdata array.
    // Be sure to follow the requirements defined in the guide/writeup
    // for quitting immediately if the user input is out-of-bounds
    // (i.e. not in the range 0 to dim_sq-1 nor -1 nor -2) as well as
    // continuing to prompt for an input if the user chooses locations
    // that have already been chosen (already marked with an X or O).
    //**********************************************************

    // Add your code here for getting input

    cin >> player_input; 

    // if user chooses AI or input val has not been selected before, if not, prompt user again
    if (player_input == -1 || tttdata[player_input] == '?')
    { 
      if (player_input != -1 && player_input != -2 && player_input < 0 && player_input > dim_sq-1)
      { 
        return -1; // quit immediately
      }

      // AI
      if (player_input == -1)
      {
        getAiChoiceAndUpdateGrid(tttdata, dim, player);
      }

      // random choice
      else if (player_input == -2) 
      {
        getRandChoiceAndUpdateGrid(tttdata, dim, player);
      }

      // if player input is within range
      else if (player_input >= 0 && player_input < dim_sq)
      { 
        // set index val in array to char of player
        tttdata[player_input] = player; 
      }

      // Show the updated board if the user eventually chose a valid location
      // (i.e. you should have quit the loop and program by now without any
      //  other output message if the user chosen an out-of-bounds input).

      // print newline before tic-tac-toe
      cout << endl; 
      printTTT(tttdata, dim); 

      //**********************************************************
      // Complete the body of the while loop to process the input that was just
      //  received to check for a winner or draw and update other status, as
      //  needed.
      //
      // To match our automated checkers' expected output, you must output
      // one of the messages defined above using *one* of the cout commands
      // (under the appropriate condition) below:
      //   cout << xwins_msg << endl;  OR
      //   cout << owins_msg << endl;  OR
      //   cout << draw_msg << endl;
      //
      // Note: Our automated checkers will examine a specific number of lines
      //  at the end of the program's output and expect to see the updated board
      //  and game status message.  You may certainly add some debug print
      //  statements during development but they will need to be removed to
      //  pass the automated checks.
      //**********************************************************

      if (checkForWinner(tttdata, dim) == 0 && player == 'X')
      { // 'X' has not won yet, so continue playing
        player = 'O'; // change player to 'O'
        if (checkForDraw(tttdata, dim) == true) // check if game is a draw 
        {
          cout << draw_msg << endl;
          done = true;
        }
      }

      else // if player is 'O' and no one has won yet, change to 'X'
      { 
        player = 'X';
        if (checkForDraw(tttdata, dim) == true) // check if game is a draw
        { 
          cout << draw_msg << endl;
          done = true; // set done to true, exit loop
        }
      }
      
      if (checkForWinner(tttdata, dim) == 1) // check if player 'X' won
      { 
        cout << xwins_msg << endl;
        done = true; // set done to true, exit loop
      }

      else if (checkForWinner(tttdata, dim) == 2)  // check if player 'O' won
      {
        cout << owins_msg << endl;
        done = true; // set done to true, exit loop
      }
    }

  } // end while
  return 0;
}