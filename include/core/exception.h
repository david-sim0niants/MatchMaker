#pragma once

#include <stdexcept>

namespace matchmaker::core {

class Exception : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

}
