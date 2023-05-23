/*
mazeio.cpp
*/

#include <iostream>
#include <fstream>
#include "mazeio.h"

using namespace std;

/*************************************************
 * read_maze:
 * Read the maze from the given filename into a
 *  2D dynamically allocated array.
 *
 * Return the pointer to that array.
 * Return NULL (a special address) if there is a problem,
 * opening the file or reading in the integer sizes.
 *
 * The first argument is the filename of the maze input.
 *  You should use an ifstream to open and read from this
 *  file.
 *
 * We also pass in two pointers to integers. These are
 * output parameters (i.e. declared by the caller and
 * passed-by-reference for this function to fill in).
 * Fill the integers pointed to by these arguments
 * with the number of rows and columns
 * read (the first two input values).
 *
 *************************************************/
char** read_maze(char* filename, int* rows, int* cols)
{
  // *** You complete **** CHECKPOINT 1
  ifstream ifile(filename); // declare ifstream ifile to take in filename
  if (ifile.fail()) // able to open file?
  {
    return NULL; // if not return NULL
  }
  int x, y; // declare int vars for row/col in file
  ifile >> x >> y; // read in maze dimensions
  if (ifile.fail()) // able to read in two ints?
  {
    return NULL; // if not, return NULL
  }

  *rows = x; *cols = y; // set row/col vals to x and y in file
  
  char** maze = new char*[*rows]; // dynamically allocate 2D array for maze
  char buf[*rows][*cols]; // declare buf array

  for (int i = 0; i < *rows; i++)
  {
    maze[i] = new char[*cols];
    for (int j = 0; j < *cols; j++)
    {
      ifile >> buf[i][j]; // read in chars from file to buf array
      maze[i][j] = buf[i][j]; // copy contents from buf array into maze
    }
  }

  ifile.close(); // close ifile
  return maze; // returns 2D dynamically allocated array
}

/*************************************************
 * Print the maze contents to the screen in the
 * same format as the input (rows and columns, then
 * the maze character grid).
 *************************************************/
void print_maze(char** maze, int rows, int cols)
{
  // *** You complete **** CHECKPOINT 1
  cout << rows << " " << cols << endl; // print maze dimensions
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      cout << maze[i][j];
    }
    cout << endl;
  }
}

