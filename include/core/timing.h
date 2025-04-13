#pragma once

#include <chrono>

#include "misc/printing.h"

namespace matchmaker::core {

using namespace std::chrono_literals;
using Time = std::chrono::milliseconds;
using Duration = std::chrono::milliseconds;

}

namespace matchmaker::misc {

template<> inline void print_to(std::ostream& os, const core::Time& time)
{
    print_to(os, time.count(), "ms");
}

}
