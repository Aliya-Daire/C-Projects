#ifndef USER_H
#define USER_H
#include <string>
#include <vector>
#include <iostream>

// Type to model content IDs
typedef int CID_T;

struct User {
  // Data members
  std::string uname; // unique username 
  int ratingLimit; // rating limit of the highest (most-restrictive) content this user can watch - used in conj w rating value (0-4)
  std::vector<CID_T> history; // list of the IDs (type int CID) of the content the user has watched
  // Convenience constructor
  User(std::string username, int rLimit = 0)
    : uname(username), ratingLimit(rLimit)
  {}

  /**
    * @brief Check is this User has watchd the specified Content
    * 
    * @param contentID ID of the content to check
    * @return true if the User HAS watched the content
    * @return false otherwise
  */
  bool haveWatched(CID_T contentID);

  /**
    * @brief Adds the given ContentID to the viewing history of this User
    *        Does not check if the Content ID has already been viewed.
    * 
    * @param contentID ID of the content to add
    */
  void addToHistory(CID_T contentID);
};


#endif