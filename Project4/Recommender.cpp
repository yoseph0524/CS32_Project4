#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include "User.h"
using namespace std;

// comparing compatibility score
bool operator>(const MovieAndRank& a, const MovieAndRank& b)
{
    if (a.compatibility_score > b.compatibility_score)
        return true;
    else
        return false;
}
bool operator<(const MovieAndRank& a, const MovieAndRank& b)
{
    if (a.compatibility_score < b.compatibility_score)
        return true;
    else
        return false;
}
bool operator==(const MovieAndRank& a, const MovieAndRank& b)
{
    if (a.compatibility_score == b.compatibility_score)
        return true;
    else
        return false;
}

bool operator!=(const Movie& a, const Movie& b)
{
    if (a.get_id() != b.get_id())
        return true;
    else
        return false;
}

Recommender::Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database)
{
    ud = &user_database;
    md = &movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    vector<MovieAndRank> rec;

    if (movie_count <= 0)
        return rec;

    User* user = ud->get_user_from_email(user_email);
    if (user == nullptr)
        return rec;

    vector<string> watch_history = user->get_watch_history();
    
    map<string,int> scores;
    for (string s : watch_history)
    {
        Movie* movie = md->get_movie_from_id(s);
        movie->get_directors();
        for(string director : movie->get_directors())
        {
            for (Movie* pot : md->get_movies_with_director(director))
            {
                if (pot != movie)
                {
                    scores[pot->get_id()]+=20;
                }
            }
        }
        for(string actor : movie->get_actors())
        {
            for (Movie* pot : md->get_movies_with_actor(actor))
            {
                if (pot != movie)
                {
                    scores[pot->get_id()]+=30;
                }
            }
        }
        for(string genre : movie->get_genres())
        {
            for (Movie* pot : md->get_movies_with_genre(genre))
            {
                if (pot != movie)
                {
                    scores[pot->get_id()]+=1;
                }
            }
        }
    }

    for (auto it = scores.begin(); it != scores.end();++it)
    {
        MovieAndRank m(it->first,it->second);
        rec.push_back(m);
    }
    

    quickSort(rec,0, rec.size()-1, md);

    if (rec.size() > movie_count)
    {
        rec.erase(rec.begin() + movie_count, rec.end());
    }

    return rec;
}


//sort the array in alphabetical order just in case of tie
//using efficient sort algorithm
void Recommender::quickSort(vector<MovieAndRank>& arr, int left, int right, const MovieDatabase* temp) const
{

    if (left < right)
    {
        int pivotIndex = left + (right - left) / 2;
        int newPivotIndex = left;
        MovieAndRank pivotValue = arr[pivotIndex];
        std::swap(arr[pivotIndex], arr[right]);

        for (int i = left; i < right; i++) {
            if (compareMovies(arr[i], pivotValue, temp)) {
                std::swap(arr[i], arr[newPivotIndex]);
                newPivotIndex++;
            }
        }
        std::swap(arr[newPivotIndex], arr[right]);


        quickSort(arr, left, newPivotIndex - 1, temp);
        quickSort(arr, newPivotIndex + 1, right, temp);
    }
}

// comaping movies
bool Recommender::compareMovies(const MovieAndRank& a, const MovieAndRank& b, const MovieDatabase* temp)
{
    if (a.compatibility_score != b.compatibility_score)
    {
        return a.compatibility_score > b.compatibility_score;
    }

    if (temp->get_movie_from_id(a.movie_id)->get_rating() != temp->get_movie_from_id(b.movie_id)->get_rating())
    {
        return temp->get_movie_from_id(a.movie_id)->get_rating() >  temp->get_movie_from_id(b.movie_id)->get_rating();
    }

    return temp->get_movie_from_id(a.movie_id)->get_title() > temp->get_movie_from_id(b.movie_id)->get_title();

}
