#include "seed.h"

#include <cstdlib>
#include <iostream>
#include <thread>

void print_usage(const char *name)
{
    std::cerr << "Usage: " << name << " <a_player_name> <b_player_name>\n";
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *a_player = argv[1];
    const char *b_player = argv[2];

    const char *rps[] = {"Rock", "Paper", "Scissors"};

    seed();

    const int a_choice = rand() % 3;
    const int b_choice = rand() % 3;

    std::cout << a_player << " choice: " << rps[a_choice] << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    std::cout << b_player << " choice: " << rps[b_choice] << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    if (a_choice == b_choice)
        std::cout << a_player << ' ' << b_player;
    else if ((b_choice + 1) % 3 == a_choice)
        std::cout << a_player;
    else
        std::cout << b_player;
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
