/*
queue.cpp
*/

#include "queue.h"

//Constructor. maxlen must be as large as the total number
// of Locations that will ever be entered into this Queue.
Queue::Queue(int maxlen) 
{
  head = 0;
  tail = 0;
  // *** You complete **** CHECKPOINT 3

  // need storage!!
  contents = new Location[maxlen]; // initialize contents array
}

//Destructor. releases resources. C++ will call it automatically.
Queue::~Queue() 
{
  // *** You complete **** CHECKPOINT 3
  delete [] contents; // deallocate contents array
}

// Insert a new Location at the end/back of our list
void Queue::add_to_back(Location loc) 
{
  // *** You complete **** CHECKPOINT 3
  contents[tail] = loc; // set contents array at tail val to given Location
  tail++; // increment tail
}

// Return and "remove" the oldest Location not already extracted
Location Queue::remove_from_front() 
{
  // *** You complete **** CHECKPOINT 3
  Location temp; // declare temp Location val
  temp = contents[head]; // initialize temp to contents array at head val
  head++; // increment head
  return temp; // return Location
}

//Is this Queue empty? (did we extract everything added?)
//This is complete, you don't need to change it.
bool Queue::is_empty() 
{
  return head == tail;
}

