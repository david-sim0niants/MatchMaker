#pragma once

#include <string>
#include <vector>

namespace matchmaker::core {

struct UserInfo {
    std::string_view username;
    std::string name, last_name;
    std::vector<std::string> preferred_games;
};

}
