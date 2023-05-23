/*
maze.cpp
*/

#include <iostream>
#include "mazeio.h"
#include "queue.h"

using namespace std;

// Prototype for maze_search, which you will fill in below.
int maze_search(char**, int, int);

// Add other prototypes here for any functions you wish to use
int valid_maze(char**, int, int);
int invalid_char_in_maze(char**, int, int);

// main function to read, solve maze, and print result
int main(int argc, char* argv[]) 
{
  int rows, cols, result;
  char** mymaze = NULL;
  const char* invalid_char_message = "Error, invalid character.";
  const char* invalid_maze_message = "Invalid maze.";
  const char* no_path_message = "No path could be found!";

  if (argc < 2)
  {
    cout << "Please provide a maze input file" << endl;
    return 1;
  }

  // set mymaze to read_maze function w/ second command line argument, and address of the row and col vals
  mymaze = read_maze(argv[1], &rows, &cols); // <---TASK: COMPLETE THIS FOR CHECKPOINT 1

  // For checkpoint 2 you should check the validity of the maze
  // You may do so anywhere you please and can abstract that
  // operation with a function or however you like.

  // check maze validity
  if (valid_maze(mymaze, rows, cols) == 1)
  {
    cout << invalid_maze_message << endl;
    return 1;
  }

  // check for valid chars in maze
  if (invalid_char_in_maze(mymaze, rows, cols) == 1)
  {
    cout << invalid_char_message << endl;
    return 1;
  }

  //================================
  // When working on Checkpoint 4, you will need to call maze_search
  // and output the appropriate message or, if successful, print
  // the maze.  But for Checkpoint 1, we print the maze, regardless.

  // initialize result w/ maze_search function
  result = maze_search(mymaze, rows, cols);

  // if there is no path, print message and quit program
  if (result == 0)
  {
    cout << no_path_message << endl;
    return 1;
  }

  // only print maze if a path is found
  if (result == 1)
  {
    print_maze(mymaze, rows, cols);
  }

  //================================
  // ADD CODE BELOW 
  // to delete all memory that read_maze allocated: CHECKPOINT 2

  // deallocate maze at end of main
  for (int i = 0; i < rows; i++)
  {
    delete [] mymaze[i];        
  }
  delete [] mymaze;

  return 0;
}

/**************************************************
 * Attempt to find shortest path and return:
 *  1 if successful
 *  0 if no path exists
 *
 * If path is found fill it in with '*' characters
 *  but don't overwrite the 'S' and 'F' cells
 * NOTE: don't forget to deallocate memory in here too!
 *************************************************/
int maze_search(char** maze, int rows, int cols)
{
  // *** You complete **** CHECKPOINT 4

  // 1. for badly-formatted maze
  if (valid_maze(maze, rows, cols) == 1)
  {
    return -1;
  }

  // 2. create queue w/ appropriate size rows*cols
  Queue q(rows*cols);

  // 3. dynamically allocate + initialize predecessor/explored arrays
  // declare and initialize Location var for pred array
  Location init_pred; 
  init_pred.row = -1; init_pred.col = -1;
  Location** predecessor = new Location*[rows];
  for (int i = 0; i < rows; i++)
  {
    predecessor[i] = new Location[cols];
    for (int j = 0; j < cols; j++)
    {
      predecessor[i][j] = init_pred;
    }
  }

  // explored is bool array initialized to false
  bool** explored = new bool*[rows];
  for (int i = 0; i < rows; i++)
  {
    explored[i] = new bool[cols];
    for (int j = 0; j < cols; j++)
    {
      explored[i][j] = 0;
    }
  }

  // 4. perform BFS algorithm
  Location start, curr; // declare Location vars for BFS
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      if (maze[i][j] == 'S')
      {
        // mark start location as explored
        explored[i][j] = 1; // change explored array at index where 'S' is found from false to true
        start.row = i; start.col = j; // set row/col at start to index vals
        q.add_to_back(start); // add start location to back of queue (first Location in queue)
      }
    }
  }
  while (!q.is_empty()) // while q is not empty
  { // do:
    curr = q.remove_from_front(); // set loc to extract the item from front of q w/ remove_from_front func
    // for each neighbor (i.e., N, W, S, and E) of loc do:
    // north
    Location north_of_curr; // declare Location var for neighbor
    north_of_curr.row = curr.row - 1; north_of_curr.col = curr.col; // initialize row/col val
    if (curr.row != 0 && maze[north_of_curr.row][north_of_curr.col] != '#' && explored[north_of_curr.row][north_of_curr.col] == 0) 
    { // if in bounds, not a barrier, and explored[][] = 0
      if (maze[north_of_curr.row][north_of_curr.col] == 'F')
      { // at end of maze
        while (maze[curr.row][curr.col] != 'S')
        { // trace back through predecessor array and mark path w/ *'s
          maze[curr.row][curr.col] = '*';
          curr = predecessor[curr.row][curr.col]; // update curr
        }
        // deallocate predecessor/explored arrays before program ends
        for (int i = 0; i < rows; i++)
        {
          delete [] predecessor[i];
        }
        delete [] predecessor;

        for (int i = 0; i < rows; i++)
        {
          delete [] explored[i]; 
        }
        delete [] explored;
        return 1;
      }
      else
      { // if not at end of maze yet:
        explored[north_of_curr.row][north_of_curr.col] = 1; // set explored array at Location to 1
        q.add_to_back(north_of_curr); // add to back of queue
        predecessor[north_of_curr.row][north_of_curr.col] = curr; // update predecessor array
      }
    }
    // west
    Location west_of_curr; // declare Location var for neighbor
    west_of_curr.row = curr.row; west_of_curr.col = curr.col - 1; // initialize row/col vals
    if (curr.col != 0 && maze[west_of_curr.row][west_of_curr.col] != '#' && explored[west_of_curr.row][west_of_curr.col] == 0)
    { // if Location is in bounds, not a barrier, and not explored
      if (maze[west_of_curr.row][west_of_curr.col] == 'F')
      { // if found end of maze:
        while (maze[curr.row][curr.col] != 'S')
        { // trace through predecessor array and mark path w *'s
          maze[curr.row][curr.col] = '*';
          curr = predecessor[curr.row][curr.col];
        }
        // deallocate predecessor and explored arrays before program ends
        for (int i = 0; i < rows; i++)
        {
          delete [] predecessor[i];
        }
        delete [] predecessor;

        for (int i = 0; i < rows; i++)
        {
          delete [] explored[i]; 
        }
        delete [] explored;
        return 1;
      }
      else
      { // if not at end of maze yet:
        explored[west_of_curr.row][west_of_curr.col] = 1; // change explored array at Location to true
        q.add_to_back(west_of_curr); // add to back of queue
        predecessor[west_of_curr.row][west_of_curr.col] = curr; // update predecessor array
      }
    }
    // south
    Location south_of_curr; // declare Location var
    south_of_curr.row = curr.row + 1; south_of_curr.col = curr.col; // initialize row/col vals
    if (curr.row != rows - 1 && maze[south_of_curr.row][south_of_curr.col] != '#' && explored[south_of_curr.row][south_of_curr.col] == 0)
    { // if Location is in bounds, not a barrier, and not explored
      if (maze[south_of_curr.row][south_of_curr.col] == 'F')
      { // if found end of maze:
        while (maze[curr.row][curr.col] != 'S')
        { // go through predecessory array and mark path w/ *'s
          maze[curr.row][curr.col] = '*';
          curr = predecessor[curr.row][curr.col]; // update curr
        }
        // delete predecessor/explored arrays' memory before program ends
        for (int i = 0; i < rows; i++)
        {
          delete [] predecessor[i];
        }
        delete [] predecessor;

        for (int i = 0; i < rows; i++)
        {
          delete [] explored[i]; 
        }
        delete [] explored;
        return 1;
      }
      else
      { // if not found end of maze yet:
        explored[south_of_curr.row][south_of_curr.col] = 1; // set explored array at Location to 1
        q.add_to_back(south_of_curr); // add Location to back of queue
        predecessor[south_of_curr.row][south_of_curr.col] = curr; // update predecessor array
      }
    }
    // east
    Location east_of_curr; // declare Location var
    east_of_curr.row = curr.row; east_of_curr.col = curr.col + 1; // set row/col vals
    if (curr.col != cols - 1 && maze[east_of_curr.row][east_of_curr.col] != '#' && explored[east_of_curr.row][east_of_curr.col] == 0)
    { // if Location is in bounds, not a barrier, and not explored
      if (maze[east_of_curr.row][east_of_curr.col] == 'F')
      { // if found end of maze
        while (maze[curr.row][curr.col] != 'S')
        { // trace through predecessor array and mark path w *'s
          maze[curr.row][curr.col] = '*';
          curr = predecessor[curr.row][curr.col]; // update curr
        }
        // deallocate predecessor/explored arrays before end of program
        for (int i = 0; i < rows; i++)
        {
          delete [] predecessor[i];
        }
        delete [] predecessor;
        for (int i = 0; i < rows; i++)
        {
          delete [] explored[i]; 
        }
        delete [] explored;
        return 1;
      }
      else
      { // if end of maze not found yet: 
        explored[east_of_curr.row][east_of_curr.col] = 1; // change explored array
        q.add_to_back(east_of_curr); // add Location to back of queue
        predecessor[east_of_curr.row][east_of_curr.col] = curr; // update predecessor array
      }
    }
  }

  // 6. deallocate predecessor/explored arrays
  for (int i = 0; i < rows; i++)
  {
    delete [] predecessor[i];
  }
  delete [] predecessor;

  for (int i = 0; i < rows; i++)
  {
    delete [] explored[i]; 
  }
  delete [] explored;

  // deallocate maze array
  for (int i = 0; i < rows; i++)
  {
    delete [] maze[i];        
  }
  delete [] maze;
  
  return 0; 
}

int valid_maze(char** maze, int rows, int cols)
{
  // declare/initialize start and finish count vars
  int s_count = 0;
  int f_count = 0;
  // iterate through rows/cols of maze
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      if (maze[i][j] == 'S')
      { // when start is found in maze
        s_count++; // increment start count
      }
    }
  }
  // iterate through rows/cols of maze
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      if (maze[i][j] == 'F')
      { // when finish is found in maze
        f_count++; // increment finish count
      }
    }
  }
  if (s_count == 1 && f_count == 1)
  { // if only one start and one finish
    return 0;
  }
  else
  {
    // deallocate maze array before ending program
    for (int i = 0; i < rows; i++)
    {
      delete [] maze[i];        
    }
    delete [] maze;
    return 1;
  }
}

int invalid_char_in_maze(char** maze, int rows, int cols)
{
  int count = 0;
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      if (maze[i][j] != 'S' && maze[i][j] != 'F' && maze[i][j] != '.' && maze[i][j] != '#')
      {
        count++;
      }
    }
  }
  if (count == 0)
  {
    return 0;
  }
  else
  {
    // deallocate maze array before ending program
    for (int i = 0; i < rows; i++)
    {
      delete [] maze[i];        
    }
    delete [] maze;
    return 1;
  }
}
