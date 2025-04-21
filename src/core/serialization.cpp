#include "core/serialization.h"
#include "core/user.h"

#include <algorithm>

namespace matchmaker::core {

static void trim(std::string& str);

UserSerializer::UserSerializer(std::ostream& os) : os(os)
{
}

void UserSerializer::write_username(std::string_view username)
{
    os << username << std::endl;
}

void UserSerializer::write_name(std::string_view name)
{
    os << name << ' ';
}

void UserSerializer::write_last_name(std::string_view name)
{
    os << name << ' ';
}

void UserSerializer::write_preferred_game(std::string_view game_name)
{
    if (first_preferred_game_written)
        os << " , ";
    os << game_name;
    first_preferred_game_written = true;
}

void UserSerializer::finish()
{
    os << std::endl;
}

UserDeserializer::UserDeserializer(std::istream& is) : is(is)
{
}

void UserDeserializer::read_username(char *username, std::size_t& username_length)
{
    is.getline(username, User::max_username_length);
    username_length =
        std::find_if_not(username, username + User::max_username_length, ::isprint) - username;
}

void UserDeserializer::read_name(std::string& name)
{
    is >> name;
}

void UserDeserializer::read_last_name(std::string& last_name)
{
    is >> last_name;
    start_reading_preferred_games();
}

bool UserDeserializer::read_preferred_game(std::string& preferred_game)
{
    std::getline(preferred_games_iss, preferred_game, ',');
    trim(preferred_game);
    return (bool)preferred_games_iss;
}

void UserDeserializer::start_reading_preferred_games()
{
    std::string preferred_games_line;
    std::getline(is, preferred_games_line, '\n');
    preferred_games_iss = std::istringstream(preferred_games_line);
}

static void trim(std::string& str)
{
    auto l_it = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto r_it = std::find_if_not(str.rbegin(), std::reverse_iterator(l_it), ::isspace).base();
    if (l_it != str.begin())
        std::copy(l_it, r_it, str.begin());
    str.resize(r_it - l_it);
}

}
