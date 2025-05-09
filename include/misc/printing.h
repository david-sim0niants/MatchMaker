#pragma once

#include <ostream>
#include <sstream>
#include <type_traits>

namespace matchmaker::misc {

template<typename T, typename Enable = void>
struct Printing {
    static inline void print_to(std::ostream& os, const T& obj)
    {
        os << "<unpresentable>";
    }
};

template<typename T>
struct Printing<T, std::enable_if_t<
    std::is_integral_v<T> || std::is_floating_point_v<T> ||
    std::is_same_v<std::decay_t<T>, std::string> ||
    std::is_same_v<std::decay_t<T>, std::string_view> ||
    std::is_same_v<std::decay_t<T>, const char*> ||
    std::is_same_v<std::decay_t<T>, char*> >
> {
    static inline void print_to(std::ostream& os, const T& obj)
    {
        os << obj;
    }
};

template<typename T>
inline void print_to(std::ostream& os, const T& obj)
{
    return Printing<T>::print_to(os, obj);
}

inline void print_to(std::ostream& os, const bool& b)
{
    os << (b ? "true" : "false");
}

inline void print_to(std::ostream& os, unsigned char c)
{
    os << static_cast<unsigned int>(c);
}

template<typename... Ts>
std::enable_if_t<(sizeof...(Ts) > 1), void> print_to(std::ostream& os, const Ts&... objs)
{
    (print_to(os, objs), ...);
}

template<typename T>
inline std::string stringify(const T& obj)
{
    std::ostringstream oss;
    print_to(oss, obj);
    return std::move(oss).str();
}

template<typename... Ts>
inline std::enable_if_t<(sizeof...(Ts) > 1), std::string> stringify(const Ts&... objs)
{
    std::ostringstream oss;
    (print_to(oss, objs), ...);
    return std::move(oss).str();
}

}
