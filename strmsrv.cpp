
#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;

// To do - Complete this function
StreamService::StreamService()
{ 
  // Update as needed
  cUser_ = NULL;
}

// To do - Complete this function
StreamService::~StreamService()
{
  for (unsigned int i = 0; i < users_.size(); i++)
  {
    delete users_[i];
  }
  for (unsigned int i = 0; i < content_.size(); i++)
  {
    delete content_[i];
  }
}

// Complete - Do not alter
void StreamService::readAndParseData(std::istream& is, Parser& p)
{
  p.parse(is, content_, users_);
  cout << "Read " << content_.size() << " content items." << endl;
  cout << "Read " << users_.size() << " users." << endl;
}

// To do - Complete this function
// sets the current user to the given username
// allow a user to login - track who is the "current" user
void StreamService::userLogin(const std::string& uname)
{
  if(cUser_ != NULL)
  { // if another user is already logged in (i.e. they should logout first)
    throw std::runtime_error("Another user is logged in");
  }
  if (getUserIndexByName(uname) == -1)
  {
    throw std::invalid_argument("Invalid argument");
  }
  cUser_ = users_[getUserIndexByName(uname)];
}

// To do - Complete this function
// Sets the appropriate state to indicate no user is
// logged in. If no user was logged in, just do nothing (return)
void StreamService::userLogout()
{ // allow a user to logout
  cUser_ = NULL; // reset the cUser_ pointer to an appropriate value (think of what that should be)
}

// To do - Complete this function
std::vector<CID_T> StreamService::searchContent(const std::string& partial) const
{
  std::vector<CID_T> results;
  for(size_t i = 0; i < content_.size(); i++)
  {
    // TO DO - modify this to also push back when the string 'partial'
    //  is contained in the name of the current content. Lookup the string
    //  class documentation to find an appropriate function that can be used
    //  to do this simply.
    if(partial == "*" || content_[i]->name().find(partial) != string::npos)
    {
      results.push_back(i);
    }        
  }
  return results;
}

// Complete - Do not alter
std::vector<CID_T> StreamService::getUserHistory() const
{
  throwIfNoCurrentUser();
  return cUser_->history;
}

// To do - Complete this function
void StreamService::watch(CID_T contentID)
{ // watch a movie or series - update viewing history of the user and the chosen content
  throwIfNoCurrentUser();
  if (!isValidContentID(contentID))
  {
    throw ReviewRangeError("ContentID is not valid.");
  }
  if (content_[contentID]->rating() > cUser_->ratingLimit)
  {
    throw RatingLimitError("Rating limit out of range.");
  }
  if (cUser_->haveWatched(contentID) == false)
  {
    cUser_->history.push_back(contentID);
    content_[contentID]->addViewer(cUser_->uname); // push back viewer to contents_ vector
  }
}

// To do - Complete this function
  // Add a review with the given number of stars to the specified
  // content. We will NOT worry about a user reviewing content
  // more than once.

  // contentID ContentID to review
  // numStars Number of stars for the review (must be 0-5)
void StreamService::reviewShow(CID_T contentID, int numStars)
{ // add a review with a given number of stars to particular content
  throwIfNoCurrentUser();
  if (!isValidContentID(contentID) || (numStars < 0 || numStars > 5))
  {
    throw ReviewRangeError("ContentID is not valid or the number of stars is out of the range 0-5.");
  }
  content_[contentID]->review(numStars);
}

// To do - Complete this function
CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
  // Change this when you are ready, but for now it is a 
  // dummy implementation

  // recommend similar Content based on the viewing history of other 
  // users who viewed a particular Content item

  // the current user Uq will ask for suggestions similar to the specific 
  // Content ID, CID

  // find the set of users who also watched the content 
  if (cUser_ == NULL)
  {
    throw UserNotLoggedInError("No current user.");
  }
  if (!isValidContentID(contentID))
  {
    throw ReviewRangeError("Invalid contentID");
  }
  int count = 0;
  int mostWatched = 0;
  vector<User*> viewed;
  int mostWatchedID = -1;
  for (size_t i = 0; i < users_.size(); i++)
  {
    if (users_[i]->uname == cUser_->uname)
    {
      continue;
    }
    if (users_[i]->haveWatched(contentID) == true)
    {
      viewed.push_back(users_[i]);
      //cout << users_[i]->uname;
    }   
  }
  for (unsigned int i = 0; i < content_.size(); i++)
  {
    if (cUser_->haveWatched(i))
    { // already watched
      continue;
    }
    if (i == contentID){
      continue;
    }
    else
    {
      count = 0;
      for (unsigned int j = 0; j < viewed.size(); j++)
      {
        if (viewed[j]->haveWatched(i) == true)
        {
          count++;
        }
      }
      //cout << count << " " << i << endl;
        if (count > mostWatched)
        {
          
          mostWatched = count;
          mostWatchedID = i;
        }
    }
  }
    return mostWatchedID;
  // for (size_t i = 0; i < users_.size(); i++)
  // {
  //   while (users_[i]->haveWatched(contentID) != cUser_->haveWatched(contentID))
  //   {
  //     if (users_[i]->haveWatched(contentID) == true)
  //     {
  //       count++;
  //     }
  //     if (count > mostWatched)
  //     {
  //       mostWatched = count;
  //     }
  //     return contentID;
  //   }
  // }
  // if no Content items meet the criteria 
  return -1;
}

// To do - Complete this function
void StreamService::displayContentInfo(CID_T contentID) const
{
  // Do not alter this
  if(!isValidContentID(contentID))
  {
    throw std::invalid_argument("Watch: invalid contentID");
  }

  // Call the display ability of the appropriate content object
  content_[contentID]->display(cout);
}

// Complete - Do not alter
bool StreamService::isValidContentID(CID_T contentID) const
{
  return (contentID >= 0) && (contentID < (int)content_.size());
}

// Complete - Do not alter
void StreamService::throwIfNoCurrentUser() const
{
  if(cUser_ == NULL)
  {
    throw UserNotLoggedInError("No user is logged in");
  }
}

// Complete - Do not alter
int StreamService::getUserIndexByName(const std::string& uname) const
{
  for(size_t i = 0; i < users_.size(); i++)
  {
    if(uname == users_[i]->uname) 
    {
      return (int)i;
    }
  }
  return -1;
}
