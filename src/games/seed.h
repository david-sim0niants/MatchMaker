#include <cstdlib>
#include <ctime>

static void seed()
{
    const char *seed = getenv("SEED");
    try {
        if (seed) {
            srand(std::atoi(seed));
            return;
        }
    } catch (...) {
    }
    srand(time(nullptr));
}
