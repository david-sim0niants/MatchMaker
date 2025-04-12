#pragma once

#include <optional>
#include <type_traits>

namespace matchmaker {

/** Defers the function (or callable) call at scope end where this object exists.
 * Uses RAII principle to accomplish this by making the call in its destructor. */
template<typename F, std::enable_if_t<std::is_invocable_v<F>, int> = 0>
class Defer {
public:
    Defer(F f) : f(f)
    {
    }

    inline void cancel()
    {
        f.reset();
    }

    ~Defer()
    {
        if (f) (*f)();
    }
private:
    std::optional<F> f;
};

}

#define DEFER_INTERNAL_CONCAT_IMPL(x, y) x##y
#define DEFER_INTERNAL_CONCAT(x, y) DEFER_INTERNAL_CONCAT_IMPL(x, y)
#define DEFER_CALL(f) ::matchmaker::Defer DEFER_INTERNAL_CONCAT(_deferred_call_handle_, __LINE__) (f)
#define DEFER(...) DEFER_CALL([&](){ __VA_ARGS__; })
