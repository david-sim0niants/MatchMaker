#pragma once

#include <istream>
#include <ostream>
#include <sstream>

namespace matchmaker::core {

class UserSerializer {
public:
    explicit UserSerializer(std::ostream& os);

    void write_username(std::string_view username);
    void write_name(std::string_view name);
    void write_last_name(std::string_view last_name);
    void write_preferred_game(std::string_view game_name);
    void finish();

private:
    std::ostream& os;
    bool first_preferred_game_written = false;
};

class UserDeserializer {
public:
    explicit UserDeserializer(std::istream& is);

    void read_username(char *username, std::size_t& username_length);
    void read_name(std::string& name);
    void read_last_name(std::string& last_name);
    bool read_preferred_game(std::string& preferred_game);

private:
    void start_reading_preferred_games();

    std::istream& is;
    std::istringstream preferred_games_iss;
};

}
