// Complete me
#include "customparser.h"
#include <iostream>
#include <sstream>

using namespace std;

const char* error_msg_1 = "Cannot read integer n";
const char* error_msg_2 = "Error in content parsing";
const char* error_msg_3 = "Error in user parsing";

// To Do - Complete this function
void CustomFormatParser::parse(std::istream& is, std::vector<Content*>& content, std::vector<User*>& users)
{
  // Erase any old contents
  content.clear();
  users.clear();

  // TO DO - Add your code below.

  // use ifstream/stringstream objects appropriately in conjunction 
  // with operator>> (where does this stop?) and getline

  int n; // number of Content items to follow
  is >> n; // is - ifile
  if (is.fail())
  {
    throw ParserError(error_msg_1);
  }
  int id; int type; string name; int num_rev; int total_stars; int rating; int num_eps; 
  string tab; string individual_user;
  stringstream ss;
  for (int i = 0; i < n; i++)
  {
    is >> id >> type; // get id and type
    if (is.fail())
    {
      throw ParserError(error_msg_2);
    }
    getline(is, tab, '\t'); // get first tab between type and title
    getline(is, name); // get entire title of content
   // cout << name << endl;
    is >> num_rev >> total_stars >> rating; // read in reviews, total stars, and rating
    // cout << "num_rev " << num_rev << " total_stars " << total_stars << " rating " << rating;
    if (is.fail())
    {
      throw ParserError(error_msg_2);
    }
    if (type == 0)
    { // movie
      Content* m_ptr = new Movie(id, name, num_rev, total_stars, rating);
      content.push_back(m_ptr);
    } 
    if (type == 1)
    { // series
      is >> num_eps;
      if (is.fail())
      {
        throw ParserError(error_msg_2);
      }
      Content* s_ptr = new Series(id, name, num_rev, total_stars, rating, num_eps);
      content.push_back(s_ptr); // dynamically allocate
    }   
      getline(is, tab); // get space between rating and users
      string line;
      getline(is, line); // get entire user line
      // cout << line << endl;
      stringstream ss(line);
      //cout << line << endl;
      while (ss >> individual_user)
      {
        content[i]->addViewer(individual_user);
        //cout << individual_user << endl;
      }
  }
  string user; int rlim;
  //cout << "user " << user << endl;
  while (is >> user >> rlim)
  {
    //cout << "user " << user << endl;
    User* u_ptr = new User(user, rlim);
    users.push_back(u_ptr); // dynamically allocate
    getline(is, tab);
    string cid;
    getline(is, cid);
   // cout << cid << endl;
    stringstream ss(cid);
    CID_T individual_id;
    while (ss >> individual_id)
    {
      u_ptr->addToHistory(individual_id);
    }
  }
}