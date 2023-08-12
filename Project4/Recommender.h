#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>

class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};


class Recommender
{
  public:
    Recommender(const UserDatabase& user_database, const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email, int movie_count) const;

  private:
    const UserDatabase* ud;
    const MovieDatabase* md;
    static bool compareMovies(const MovieAndRank& a, const MovieAndRank& b, const MovieDatabase* temp);
    void quickSort(std::vector<MovieAndRank>& arr, int left, int right, const MovieDatabase* temp) const;
};

#endif // RECOMMENDER_INCLUDED
