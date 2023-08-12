#include "UserDatabase.h"
#include "User.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

UserDatabase::UserDatabase()
{
    database = new TreeMultimap<std::string, User>;
    alreadyLoaded = false;
}
UserDatabase::~UserDatabase()
{
    cout << 8;
    delete database;
}

bool UserDatabase::load(const string& filename)
{
    if(alreadyLoaded == false)
        alreadyLoaded = true;
    else
        return false;
    
    ifstream m_UserDataBase (filename);
    if(!m_UserDataBase.is_open())
        return false;
    
    string line;
    string name, email;
    int num_movies = 0;
    vector<string> watch_history;
    while(m_UserDataBase)
    {
        getline(m_UserDataBase, line);
        if(line.empty())
        {
            if(!email.empty() && !watch_history.empty())
            {
                User user(name, email, watch_history);
                database->insert(email, user);
                name.clear();
                email.clear();
                watch_history.clear();
            }
        }
        //line always start with name
        else if (name.empty())
        {
            name = line;
        }
        //then email
        else if (email.empty())
        {
            email = line;
        }
        //then num of movies
        else if (num_movies == 0)
        {
            try
            {
                num_movies = stoi(line);
            }
            catch (const invalid_argument&)
            {
                return false;
            }
        }
        // and bunch of IDs
        else
        {
            watch_history.push_back(line);
            if(watch_history.size() == num_movies)
                num_movies = 0;
        }
        
    }
    //lastly insert and make it all empty
    if(!email.empty() && !watch_history.empty())
    {
        User user(name, email, watch_history);
        database->insert(email, user);
        
    }

    return true;
    
    
    return false;
    
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    //make it to all lower case -> case insensitive
    string lower = email;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if(database->find(lower).is_valid())
    {
        return &database->find(lower).get_value();
    }
    return nullptr;
    
}

