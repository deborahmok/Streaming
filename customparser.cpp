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
  int item, id, type, review, totalStars, rating, episode;
  string name, user;
  if(!(is >> item))
  {
    throw ParserError(error_msg_1);
  }
  for (int i = 0; i < item; i++) //going through each item
  {
    is >> id >> type;
    if(type == 0) //movie
    {
      getline(is, name); //it's reading the space from the first line still; moves down
      getline(is, name); //stores including the leading and trailing white space
      stringstream n (name); //it stores the name into n
      string temp = "";
      name = "";
      while(n >> temp) //read each word into temp, add it to name;
      {
        name += temp + " "; 
      }
      name.pop_back(); //get rid of the last space
      is >> review;
      if(is.fail())
      {
        throw ParserError(error_msg_2);
      }
      is >> totalStars;
      if(is.fail())
      {
        throw ParserError(error_msg_2);
      }
      is >> rating;
      if(is.fail())
      {
        throw ParserError(error_msg_2);
      }
      Content* movie = new Movie(id, name, review, totalStars, rating); //create a new movie object
      getline(is, user); 
      getline(is, user);
      stringstream u (user);
      while (u >> user) //adding who viewed the movie
      {
        movie->addViewer(user); 
      }
      content.push_back(movie); //add the movie object into content
    }
    else if (type == 1) //series
    {
      getline(is, name);
      getline(is, name);
      stringstream n (name);
      string temp = "";
      name = "";
      while(n >> temp) //reading in the series name
      {
        name += temp + " ";
      }
      name.pop_back();
      is >> review;
      if(is.fail())
      {
        throw ParserError(error_msg_2);
      }
      is >> totalStars;
      if(is.fail())
      {
        throw ParserError(error_msg_2);
      }
      is >> rating;
      if(is.fail())
      {
        throw ParserError(error_msg_2);
      }
      is >> episode;
      if (is.fail())
      {
        throw ParserError(error_msg_2);
      }
      Content* series = new Series(id, name, review, totalStars, rating, episode); //creating a new object for series
      getline(is, user);
      getline(is, user);
      stringstream u (user);
      while(u >> user) //reading in who viewed the series 
      {
        series -> addViewer(user);
      }
      content.push_back(series); //add the object series into content
    }
    else //if it's either movie or a serie
    {
      throw ParserError(error_msg_2);
    }
  }
  int rating_limit, temp;
  string content_id;
  while(is >> user) //while it can successfully able to read in a username
  {
    is >> rating_limit;
    if(is.fail())
    {
      throw ParserError(error_msg_3);
    }
    User* u = new User(user, rating_limit); //create a new user object
    getline(is, content_id);
    getline(is, content_id);
    stringstream c (content_id);  
    content_id = "";
    while (c >> temp) //read in what the user had watched
    {
      u -> addToHistory(temp);
    }
    users.push_back(u); //add the object user into all the other users
  }
}