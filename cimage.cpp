#include "component.h"
#include "cimage.h"
#include "bmplib.h"
#include <deque>
#include <iomanip>
#include <iostream>
#include <cmath>
// You shouldn't need other #include's - Ask permission before adding

using namespace std;

// TO DO: Complete this function
CImage::CImage(const char* bmp_filename)
{
  //  Note: readRGBBMP dynamically allocates a 3D array
  //    (i.e. array of pointers to pointers (1 per row/height) where each
  //    point to an array of pointers (1 per col/width) where each
  //    point to an array of 3 unsigned char (uint8_t) pixels [R,G,B values])

  // ================================================
  // TO DO: call readRGBBMP to initialize img_, h_, and w_;
  img_ = readRGBBMP(bmp_filename, h_, w_);
  // uint8_t*** img_ = uint8_t*** readRGBBMP - apples to apples

  // Leave this check
  if (img_ == NULL) 
  {
    throw std::logic_error("Could not read input file");
  }

  // Set the background RGB color using the upper-left pixel
  for (int i = 0; i < 3; i++) 
  {
    bgColor_[i] = img_[0][0][i];
  }

  // ======== This value should work - do not alter it =======
  // RGB "distance" threshold to continue a BFS from neighboring pixels
  bfsBgrdThresh_ = 60;

  // ================================================
  // TO DO: Initialize the vector of vectors of labels to -1

  // Allocate the outer vector
  for (int i = 0; i < h_; i++)
  {
    // allocate an inner vector of m entries for each row
    vector<int> myrow;
    for (int j = 0; j < w_; j++)
    {
      myrow.push_back(-1);
      // initialize each of the integers
    }
    labels_.push_back(myrow);
  }

  // ================================================
  // TO DO: Initialize any other data members

  // h_ and w_ are references passed so they are initialized when modified in the function
  id = 0; // unique int label
  count = 0;
  height_ = 0; width_ = 0;
  low_row = 0; low_col = 0;
}

// TO DO: Complete this function
CImage::~CImage()
{ 
  // Add code here if necessary
  // deallocate img_
  for (int i = 0; i < h_; i++)
  {
    for (int j = 0; j < w_; j++)
    {
      delete [] img_[i][j];
    }
    delete [] img_[i];
  }
  delete [] img_;
}

// Complete - Do not alter
bool CImage::isCloseToBground(uint8_t p1[3], double within) 
{
  // Computes "RGB" (3D Cartesian distance)
  double dist = sqrt(pow(p1[0] - bgColor_[0], 2) + pow(p1[1]-bgColor_[1],2) + pow(p1[2]-bgColor_[2],2));
  return dist <= within;
}

// TO DO: Complete this function
size_t CImage::findComponents()
{
  // scan through image 
  for (int i = 0; i < h_; i++)
  { // top-to-bottom
    for (int j = 0; j < w_; j++)
    { // left-to-right
      if ((!isCloseToBground(img_[i][j], bfsBgrdThresh_)) && labels_[i][j] == -1) // pixel SUFFICIENTLY far enough from the background color
      { // pause scan at unlabeled foreground pixels
        Component b = bfsComponent(i, j, id); // set bfs to Component
        components_.push_back(b); // push back Component
        id++; // increment label
      }
    }
  }
  return components_.size(); // size_t
}

// Complete - Do not alter
void CImage::printComponents() const
{
  cout << "Height and width of image: " << h_ << "," << w_ << endl;
  cout << setw(4) << "Ord" << setw(4) << "Lbl" << setw(6) << "ULRow" << setw(6) << "ULCol" << setw(4) << "Ht." << setw(4) << "Wi." << endl;
  for (size_t i = 0; i < components_.size(); i++) 
  {
    const Component& c = components_[i];
    cout << setw(4) << i << setw(4) << c.label << setw(6) << c.ulNew.row << setw(6) << c.ulNew.col << setw(4) << c.height << setw(4) << c.width << endl;
  }
}

// TODO: Complete this function
int CImage::getComponentIndex(int mylabel) const
{
  int idx = 0;
  for (unsigned int i = 0; i < components_.size(); i++)
  {
    if (components_[i].label == mylabel)
    {
      idx = i;
    }
  }
  return idx; // index of where component is in components_ vec
}

// Nearly complete - TO DO:
//   Add checks to ensure the new location still keeps
//   the entire component in the legal image boundaries
void CImage::translate(int mylabel, int nr, int nc)
{
  // Get the index of specified component
  int cid = getComponentIndex(mylabel);
  if (cid < 0) 
  {
    return;
  }
  int h = components_[cid].height; int w = components_[cid].width;

  // ==========================================================
  // ADD CODE TO CHECK IF THE COMPONENT WILL STILL BE IN BOUNDS
  // IF NOT:  JUST RETURN.

  // ensure the new Location will still keep the entire Component in the legal bounds
  if (nr + h > h_ || nc + w > w_ || nr < 0 || nc < 0) 
  { // if not in legal bounds of image, just return
    return;
  }

  // ==========================================================

  // If we reach here we assume the component will still be in bounds
  // so we update its location.
  Location nl(nr, nc);
  components_[cid].ulNew = nl;
}

// TO DO: Complete this function
void CImage::forward(int mylabel, int delta)
{
  int cid = getComponentIndex(mylabel);
  if (cid < 0 || delta <= 0) 
  {
    return;
  }
  // Add your code here

  int new_loc = delta + cid;
  if (new_loc >= components_.size())
  { // bounds checking
    new_loc = components_.size() - 1;
  }
  Component temp = components_[cid]; // create temporary Component to store label you want to move
  for (int i = cid; i < new_loc; i++)
  { // alter the order of the Component objects in the vector they are stored
    components_[i] = components_[i+1]; // shift all elements up
  }
  components_[new_loc] = temp;
}

// TO DO: Complete this function
void CImage::backward(int mylabel, int delta)
{
  int cid = getComponentIndex(mylabel);
  if (cid < 0 || delta <= 0) 
  {
    return;
  }
  // Add your code here

  int new_loc = cid - delta;
  if (new_loc < 0)
  { // bounds checking
    new_loc = 0;
  }
  Component temp = components_[cid]; // create temp Component
  for (int i = cid; i > new_loc; i--)
  { // alter the order of the Component objects in the vector they are stored
    components_[i] = components_[i-1]; // shift all elements back
  }
  components_[new_loc] = temp; // set desired Component at new Location
}

// TODO: complete this function
void CImage::save(const char* filename)
{
  // Create another image filled in with the background color
  uint8_t*** out = newImage(bgColor_);

  for (unsigned int i = 0; i < components_.size(); i++)
  { // process each component in appropriate order
    for (int j = 0; j < components_[i].height; j++)
    { // iterate through height and width of component
      for (int k = 0; k < components_[i].width; k++)
      {
        if (labels_[j + components_[i].ulOrig.row][k+components_[i].ulOrig.col] == components_[i].label) // only place FOREGROUND pixels at the correct Location given the new bounding boxel location
        { // if foreground pixel in component in original image
          // set output image pixel at that Location to img pixel at that Location
          for (int n = 0; n < 3; n++)
          { // 3d array - valgrind error
            out[j+components_[i].ulNew.row][k + components_[i].ulNew.col][n] = img_[j + components_[i].ulOrig.row][k + components_[i].ulOrig.col][n]; 
          }
        }
      }
    }
  }

  writeRGBBMP(filename, out, h_, w_);
  // Add any other code you need after this 
}

// Complete - Do not alter - Creates a blank image with the background color
uint8_t*** CImage::newImage(uint8_t bground[3]) const
{
  uint8_t*** img = new uint8_t**[h_];
  for (int r = 0; r < h_; r++) 
  {
    img[r] = new uint8_t*[w_];
    for (int c = 0; c < w_; c++) 
    {
      img[r][c] = new uint8_t[3];
      img[r][c][0] = bground[0];
      img[r][c][1] = bground[1];
      img[r][c][2] = bground[2];
    }
  }
  return img;
}

// To be completed
void CImage::deallocateImage(uint8_t*** img) const
{
  // Add your code here
  // delete unit8_t parameter
  for (int i = 0; i < h_; i++)
  {
    for (int j = 0; j < w_; j++)
    {
      delete [] img[i][j];
    }
    delete [] img[i];
  }
  delete [] img;
}

// TO DO: Complete the following function or delete this if you do not wish to use it.
/**
    * @brief Create a Component object starting with one of its pixels and
    *         then using an 8-connected neighbor BFS to find all pixels that
    *         are part of this Component.
    *
    * @param pr Row of the first pixel of a new Component to explore
    * @param pc Col of the first pixel of a new Component to explore
    * @param mylabel Label (unique integer ID) to use for this component
    * @return Component corresponding to this newly found digit
*/
Component CImage::bfsComponent(int pr, int pc, int mylabel)
{
  // Arrays to help produce neighbors easily in a loop
  // by encoding the **change** to the current location.
  // Goes in order N, NW, W, SW, S, SE, E, NE
  int neighbor_row[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
  int neighbor_col[8] = {0, -1, -1, -1, 0, 1, 1, 1};

  // declare/initialize variables
  deque<Location> queue; 
  Location start(pr, pc); // initialize Location var with row/col of first pixel to explore
  labels_[pr][pc] = mylabel;
  queue.push_back(start);
  Location loc;
  Location ul; ul.row = low_row; ul.col = low_col;
  int min_r = h_; int min_c = w_;
  int max_r = 0; int max_c = 0;

  // BFS:
  while (!queue.empty()) // while queue is not empty
  { // do: 
    loc = queue.front(); // set loc to front of q
    // update upper left row/col
    queue.pop_front(); // extract item from front of queue
    labels_[pr][pc] = mylabel; 
    // checks to update max and min rows and cols
    if (loc.col > max_c)
    {
      max_c = loc.col;
    }
    if (loc.col < min_c)
    {
      min_c = loc.col;
    }
    if (loc.row > max_r)
    {
      max_r = loc.row;
    }
    if (loc.row < min_r)
    {
      min_r = loc.row;
    }
    for (int i = 0; i < 8; i++)
    { // for each neighbor (i.e. N, NW, W, SW, S, SE, E, NE) of loc
      Location n;
      n.row = loc.row + neighbor_row[i]; n.col = loc.col + neighbor_col[i];
      if (n.row < h_ && n.col < w_ && n.row >= 0 && n.col >= 0)
      { // neighbor bounds checking
        if (!isCloseToBground(img_[n.row][n.col], bfsBgrdThresh_) && labels_[n.row][n.col] == -1)
        { // check if neighbor is unlabeled fground pixel
          labels_[n.row][n.col] = mylabel; // label neighbor
          queue.push_back(n); // add to back of queue
        }
      }
    }
  }
  int bh = max_r - min_r + 1; int bw = max_c - min_c + 1; // height and width variables
  Location ul_n(min_r, min_c); // create new Location
  Component blobs(ul_n, bh, bw, mylabel); // Component ctor
  return blobs; // return Component
}

// Complete - Debugging function to save a new image
void CImage::labelToRGB(const char* filename)
{
  // multiple ways to do this -- this is one way
  vector<uint8_t[3]> colors(components_.size());
  for (unsigned int i=0; i<components_.size(); i++) 
  {
    colors[i][0] = rand() % 256;
    colors[i][1] = rand() % 256;
    colors[i][2] = rand() % 256;
  }

  for (int i = 0; i < h_; i++) 
  {
    for (int j = 0; j < w_; j++) 
    {
      int mylabel = labels_[i][j];
      if (mylabel >= 0) 
      {
        img_[i][j][0] =  colors[mylabel][0];
        img_[i][j][1] =  colors[mylabel][1];
        img_[i][j][2] =  colors[mylabel][2];
      } 
      else 
      {
        img_[i][j][0] = 0;
        img_[i][j][1] = 0;
        img_[i][j][2] = 0;
      }
    }
  }
  writeRGBBMP(filename, img_, h_, w_);
}

// Complete - Do not alter
const Component& CImage::getComponent(size_t i) const
{
  if (i >= components_.size()) 
  {
    throw std::out_of_range("Index to getComponent is out of range");
  }
  return components_[i];
}

// Complete - Do not alter
size_t CImage::numComponents() const
{
  return components_.size();
}

// Complete - Do not alter
void CImage::drawBoundingBoxesAndSave(const char* filename)
{
  for (size_t i = 0; i < components_.size(); i++)
  {
    Location ul = components_[i].ulOrig;
    int h = components_[i].height;
    int w = components_[i].width;
    for(int i = ul.row; i < ul.row + h; i++)
    {
      for (int k = 0; k < 3; k++) 
      {
        img_[i][ul.col][k] = 255-bgColor_[k];
        img_[i][ul.col+w-1][k] = 255-bgColor_[k];
      }
      // cout << "bb " << i << " " << ul.col << " and " << i << " " << ul.col+w-1 << endl; 
    }
    for (int j = ul.col; j < ul.col + w ; j++)
    {
      for(int k = 0; k < 3; k++)
      {
        img_[ul.row][j][k] = 255-bgColor_[k];
        img_[ul.row+h-1][j][k] = 255-bgColor_[k];
      }
      // cout << "bb2 " << ul.row << " " << j << " and " << ul.row+h-1 << " " << j << endl; 
    }
  }
  writeRGBBMP(filename, img_, h_, w_);
}

// Add other (helper) function definitions here