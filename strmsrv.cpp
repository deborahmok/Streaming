#include "strmsrv.h"
#include <iostream>
#include <fstream>
using namespace std;

// To do - Complete this function
StreamService::StreamService()
{
  // Update as needed
  cUser_ = nullptr;
}

// To do - Complete this function
StreamService::~StreamService()
{
  for (unsigned int i = 0; i < content_.size(); i++)
  {
    delete content_[i];
  }
  for (unsigned int i = 0; i < users_.size(); i++)
  {
    delete users_[i];
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
void StreamService::userLogin(const std::string& uname)
{
  if(cUser_ != nullptr)
  {
    throw runtime_error("User is already logged in");
  }
  if(getUserIndexByName(uname) == -1)
  {
    throw invalid_argument("User is not valid");
  }
  cUser_ = users_[getUserIndexByName(uname)]; 
}

// To do - Complete this function
void StreamService::userLogout()
{
  cUser_ = nullptr;
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
    if(partial == "*" /* update me */)
    {
      results.push_back(i);
    }        
    else
    {
      //find the specific word
      if(content_[i]->name().find(partial) != string::npos) 
      {
        results.push_back(i);
      }
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
{
  throwIfNoCurrentUser();
  if(cUser_ == NULL) 
  {
    throw UserNotLoggedInError("No user is logged in");
  }
  if(!isValidContentID(contentID))
  {
    throw range_error("User is already logged in");
  }
  if(content_[contentID]->rating() > cUser_->ratingLimit)
  {
    throw RatingLimitError("User rating not met");
  }
  else if(!cUser_->haveWatched(contentID))
  {
    content_[contentID]->addViewer(cUser_->uname);
    cUser_->addToHistory(contentID);
  }
}

// To do - Complete this function
void StreamService::reviewShow(CID_T contentID, int numStars)
{
  throwIfNoCurrentUser();
  if(!isValidContentID(contentID))
  {
    throw ReviewRangeError("Review Range Error");
  }
  if(numStars < 0 || numStars > 5)
  {
    throw ReviewRangeError("Review not valid");
  }
  else 
  {
    content_[contentID]->review(numStars);
  }
}

// To do - Complete this function
CID_T StreamService::suggestBestSimilarContent(CID_T contentID) const
{
  throwIfNoCurrentUser();
  if(!isValidContentID(contentID))
  {
    throw range_error("Invalid Content ID");
  }

  std::vector<User*> whowatched; //create a vector for who watched the contentID
  int csize = content_.size(); 
  int count[csize]; //creating a count array to find the best movie to recommend
  for(int i = 0; i < csize; i++)
  {
    count[i] = 0;
  }

  //go through all the users
  for (unsigned int i = 0; i < users_.size(); i++)
  {
    //pushback the users who watched contentID and who isn't the current user
    if(users_[i] != cUser_ && users_[i]->haveWatched(contentID))
    {
      whowatched.push_back(users_[i]);
    }
  }
  
  //for each user who watched the contentID
  for (unsigned int i = 0; i < whowatched.size(); i++)
  {
    //for each content that user[i] have watched
    for (unsigned int j = 0; j < whowatched[i]->history.size(); j++)
    {
      //if current user haven't watched what user[i] haven't watched and 
      //the movies that the user[i] watched is not the contentID
      if (!cUser_->haveWatched(whowatched[i]->history[j]) && whowatched[i]->history[j] != contentID)
      {
        //add that movie index into count array
        count[whowatched[i]->history[j]]++;
      }
    }
  }

  CID_T idx = 0;
  for (CID_T i = 0; i < csize; i++) //find the most repeated show/movie that's been watched
  {
    if (count[i] > count[idx])
    {
      idx = i;
    }
  }
  if(count[idx] == 0)
  {
    return -1;
  }
  return idx;
}

// To do - Complete this function
void StreamService::displayContentInfo(CID_T contentID) const
{
  // Do not alter this
  if(!isValidContentID(contentID))
  {
    throw std::invalid_argument("Watch: invalid contentID");
  }
  
  // Call the display abitlity of the appropriate content object
  
}

// Complete - Do not alter
bool StreamService::isValidContentID(CID_T contentID) const
{
    return (contentID >= 0) && (contentID < (int) content_.size());
}

// Complete - Do not alter
void StreamService::throwIfNoCurrentUser() const
{
    if(cUser_ == NULL){
        throw UserNotLoggedInError("No user is logged in");
    }
}

// Complete - Do not alter
int StreamService::getUserIndexByName(const std::string& uname) const
{
    for(size_t i = 0; i < users_.size(); i++){
        if(uname == users_[i]->uname) {
            return (int)i;
        }
    }
    return -1;
}
