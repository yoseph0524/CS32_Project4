#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

MovieDatabase::MovieDatabase()
{
    alreadyLoaded = false;
    m_id = new TreeMultimap<string,Movie>;
    m_director = new TreeMultimap<std::string,Movie>;
    m_actor = new TreeMultimap<std::string,Movie>;
    m_genre = new TreeMultimap<std::string,Movie>;

}

MovieDatabase::~MovieDatabase()
{
    delete m_id;
    delete m_director;
    delete m_actor;
    delete m_genre;
}

bool MovieDatabase::load(const string& filename)
{
    ifstream movieFile(filename);
    string line;
    if (!alreadyLoaded && movieFile.is_open())
    {
        while(getline(movieFile,line))
        {
            // line will always start with id, title and year
            string id = line;
            getline(movieFile, line);
            string title = line;
            getline(movieFile, line);
            string year = line;
            getline(movieFile,line);

            // then, directors
            vector<string> directors;
            string::size_type start = 0, end = 0;
            while ((end = line.find(",", start)) != string::npos) {
                directors.push_back(line.substr(start, end - start));
                start = end + 1;
            }
            directors.push_back(line.substr(start));
            getline(movieFile,line); // move onto next line

            // then, actors
            vector<string> actors;
            start = 0;
            end = 0;
            while ((end = line.find(",", start)) != string::npos) {
                actors.push_back(line.substr(start, end - start));
                start = end + 1;
            }
            actors.push_back(line.substr(start));
            getline(movieFile,line);

            // then, genres
            vector<string> genres;
            start = 0;
            end = 0;
            while ((end = line.find(",", start)) != string::npos) {
                genres.push_back(line.substr(start, end - start));
                start = end + 1;
            }
            genres.push_back(line.substr(start));
            getline(movieFile,line);
            
            // last, rating
            float rating = stof(line);
           
            //make a movie containing it
            Movie movie(id,title, year, directors, actors, genres, rating); //make movie object

            //make id lower-case -> case insensitive
            string low_id = id;
            transform(low_id.begin(), low_id.end(), low_id.begin(), ::tolower);
            m_id->insert(low_id, movie); //insert id into tree

            
            for (string s : directors)
            {
                string low = s;
                transform(low.begin(), low.end(), low.begin(), ::tolower);
                m_director->insert(low, movie); // insert directors into tree
            }
            for (string s : actors)
            {
                string low = s;
                transform(low.begin(), low.end(), low.begin(), ::tolower);
                m_actor->insert(low, movie); //insert actors into tree
            }
            for (string s : genres)
            {
                string low = s;
                transform(low.begin(), low.end(), low.begin(), ::tolower);
                m_genre->insert(low, movie);
            }
            getline(movieFile,line);
        }
        movieFile.close();
        alreadyLoaded = true;
        return true;
    }
    else
        return false;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    string madeLow = id;
    transform(madeLow.begin(), madeLow.end(), madeLow.begin(), ::tolower);
    if (m_id->find(madeLow).is_valid())
        return &m_id->find(madeLow).get_value();
    else
        return nullptr;
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    string madeLow = director;
    transform(madeLow.begin(), madeLow.end(), madeLow.begin(), ::tolower);
    vector<Movie*> mv;
    auto iterator = m_director->find(madeLow);
    while (iterator.is_valid())
    {
        Movie* m = &(iterator.get_value());
        mv.push_back(m);
        iterator.advance();
    }
    return mv;
   
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    string madeLow = actor;
    transform(madeLow.begin(), madeLow.end(), madeLow.begin(), ::tolower);
    vector<Movie*> mv;

    auto iterator = m_actor->find(madeLow);
    while (iterator.is_valid())
    {
        Movie* m = &(iterator.get_value());
        mv.push_back(m);
        iterator.advance();
    }
    return mv;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    string madeLow = genre;
    transform(madeLow.begin(), madeLow.end(), madeLow.begin(), ::tolower);
    vector<Movie*> mv;

    auto iterator = m_genre->find(madeLow);
    while (iterator.is_valid())
    {
        Movie* m = &(iterator.get_value());
        mv.push_back(m);
        iterator.advance();
    }
    return mv;
}
