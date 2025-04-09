#include "core/user.h"

#include <algorithm>

namespace matchmaker::core {

User::User(std::string_view username, std::string&& name, std::string&& last_name,
           std::vector<Game *>&& preferred_games) :
    username_length(std::min(max_username_length, std::size(username))),
    name(std::move(name)),
    last_name(std::move(last_name)),
    preferred_games(std::move(preferred_games))
{
    std::copy_n(std::data(username), username_length, std::data(this->username));
}

}
