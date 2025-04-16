#pragma once

#include <string_view>

namespace matchmaker::core {

struct ExecutableGameInfo {
    std::string_view name;
    std::string_view comm;
    std::string_view rel_path = comm;
};

}
