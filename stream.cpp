#include <iostream>
#include <fstream>
#include <cctype>
#include "strmsrv.h"
#include "customparser.h"
#include "jsonparser.h"

using namespace std;

int main(int argc, char* argv[])
{
  if(argc < 2)
  {
    cout << "Please provide a database filename." << endl;
    return 1;
  }
  const char* parser_except_msg = "Parser exception occurred";
  const char* no_user_except_msg = "No user logged in";
  const char* rating_limit_except_msg = "User not permitted to view this content";
  const char* review_range_except_msg = "Review should be between 0-5 stars";

  // Declare the parsers
  CustomFormatParser cp;
  JsonParser jp;

  // Declare the StreamService object
  StreamService strm;

  ifstream ifile(argv[1]);
  if(ifile.fail())
  {
    cout << "Cannot open file: " << argv[1] << endl;
    return 1;
  }
  // Attempt to initialize the StreamService object
  // Wrap the following block of code in a try..catch
  // sequence.  If a ParserException is thrown, print out
  // the what() message and return 1.  For any other exception that is 
  // thrown, ONLY output the string defined in `parser_except_msg` above
  // and return 1.
  string dbname(argv[1]);
  if(dbname.find(".json") != string::npos)
  {
    strm.readAndParseData(ifile, jp);
  }
  else if(dbname.find(".cus") != string::npos)
  {
    strm.readAndParseData(ifile, cp);
  }
  else
  {
    cout << "Couldn't identify file format" << endl;
    return 1;
  }
  
  // Close the file now that we are done parsing
  ifile.close();

  // Now interact with the user
  cout << "Menu:" << endl;
  cout << "===================================================" << endl;
  cout << "QUIT:              0" << endl;
  cout << "LOGIN:             1 uname" << endl;
  cout << "LOGOUT:            2" << endl;
  cout << "GET USER HISTORY:  3" << endl;
  cout << "SEARCH:            4 <* | string>" << endl;
  cout << "WATCH:             5 <content-id>" << endl;
  cout << "REVIEW:            6 <content-id> <number-of-stars>" << endl;
  cout << "SUGGEST SIMILAR:   7 <content-id>" << endl;
  cout << "===================================================" << endl;
  int option = -1;
  while(option != 0) 
  {
    cout << "\nEnter a command: " << endl;
    cin >> option;
    try 
    {
      if(option == 1) 
      {
        string uname;
        cin >> uname;
        if(cin.fail())
        {
          throw UserNotLoggedInError(no_user_except_msg);
        }
        strm.userLogin(uname);
      }
      else if(option == 2)
      {
        strm.userLogout();
      }
      else if(option == 3)
      {
        vector<CID_T> results;
        results = strm.getUserHistory();

        // Process results
        cout << "Results: " << endl;
        for(size_t i = 0; i < results.size(); i++)
        {
          strm.displayContentInfo(results[i]);
        }
      }
      else if(option == 4)
      {
        string search;
        while(isspace(cin.peek())) 
        {
          cin.get();
        }
        getline(cin, search);
        vector<CID_T> results;
        results = strm.searchContent(search);

        // Process results
        cout << "Results: " << endl;
        for(size_t i = 0; i < results.size(); i++)
        {
          strm.displayContentInfo(results[i]);
        }
      }
      else if(option == 5)
      {
        CID_T cid;
        cin >> cid;
        // Add appropriate exception handling as described
        // in the writeup at an appropriate location of your
        // choosing. Note it is fine to add a try block here 
        // (i.e. nested try blocks are legal)
        if (cin.fail())
        {
          throw RatingLimitError(rating_limit_except_msg);
        }
        strm.watch(cid);  
      }
      else if(option == 6)
      {
        CID_T cid;
        int numstars;
        cin >> cid >> numstars;
        if (cin.fail())
        {
          throw ReviewRangeError(review_range_except_msg);
        }
        // Add appropriate exception handling as described
        // in the writeup at an appropriate location of your
        // choosing. Note it is fine to add a try block here 
        // (i.e. nested try blocks are legal)
        strm.reviewShow(cid, numstars);                
      }
      else if(option == 7)
      {
        CID_T cid;
        cin >> cid;
        CID_T result = strm.suggestBestSimilarContent(cid);
        if(result == -1)
        {
          cout << "No suggestions available." << endl;
        }
        else 
        {
          strm.displayContentInfo(result);
        }
      }
      else 
      {
        option = 0;
      }
    }
    // Potentially add more catch blocks here, as necessary
    catch(ReviewRangeError(review_range_except_msg)) 
    {
      cout << "Review should be between 0-5 stars" << endl;
    }
    catch(RatingLimitError(rating_limit_except_msg))
    {
      cout << "User not permitted to view this content" << endl;
    }
    catch(UserNotLoggedInError(no_user_except_msg))
    {
      cout << "No user logged in" << endl;
    }
  }
  return 0;
}
