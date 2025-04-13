#include "seed.h"

#include <array>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <cstdlib>
#include <thread>

enum class Player
{
    None = 0,
    X,
    O
};

template<int size>
using Board = std::array<std::array<Player, size>, size>;

struct ScanResult {
    char flags = 0;

    inline void have(Player cell)
    {
        switch (cell) {
        case Player::None:
            have_empty();
            break;
        case Player::X:
            have_x();
            break;
        case Player::O:
            have_o();
            break;
        default:
            break;
        }
    }

    inline void have_x() noexcept
    {
        flags |= 0b001;
    }

    inline void have_o() noexcept
    {
        flags |= 0b010;
    }

    inline void have_empty() noexcept
    {
        flags |= 0b100;
    }

    inline bool has_x() const noexcept
    {
        return flags & 0b001;
    }

    inline bool has_o() const noexcept
    {
        return flags & 0b010;
    }

    inline bool has_empty() const noexcept
    {
        return flags & 0b100;
    }
};

template<int board_size>
class TicTacToe {
public:
    void take_turn(int i, int j)
    {
        if (nr_free_cells <= 0)
            throw std::logic_error("game finished");

        if (i >= board_size || i < 0 || j >= board_size || j < 0)
            throw std::out_of_range("out of board bounds");

        board[i][j] = current_player;
        --nr_free_cells;
        switch_player();
    }

    const Board<board_size>& get_board() const noexcept
    {
        return board;
    }

    int get_nr_free_cells() const noexcept
    {
        return nr_free_cells;
    }

    std::pair<bool, Player> check() const
    {
        Player winner = check_winner();
        if (winner != Player::None)
            return std::make_pair(true, winner);
        else
            return std::make_pair(check_draw(), Player::None);
    }

private:
    Player check_winner() const noexcept
    {
        for (int i = 0; i < board_size; ++i) {
            Player winner = check_winner_from_scan(scan_row(i));
            if (winner != Player::None)
                return winner;
        }

        for (int j = 0; j < board_size; ++j) {
            Player winner = check_winner_from_scan(scan_col(j));
            if (winner != Player::None)
                return winner;
        }

        Player winner = check_winner_from_scan(scan_primary_diagonal());
        if (winner != Player::None)
            return winner;

        winner = check_winner_from_scan(scan_secondary_diagonal());
        if (winner != Player::None)
            return winner;

        return Player::None;
    }

    bool check_draw() const noexcept
    {
        for (int i = 0; i < board_size; ++i)
            if (! check_draw_from_scan(scan_row(i)))
                return false;

        for (int j = 0; j < board_size; ++j)
            if (! check_draw_from_scan(scan_col(j)))
                return false;

        return check_draw_from_scan(scan_primary_diagonal())
            && check_draw_from_scan(scan_secondary_diagonal());
    }

    Player check_winner_from_scan(ScanResult scan_res) const noexcept
    {
        if (scan_res.has_empty() || scan_res.has_x() == scan_res.has_o())
            return Player::None;
        else if (scan_res.has_x())
            return Player::X;
        else
            return Player::O;
    }

    bool check_draw_from_scan(ScanResult scan_res) const noexcept
    {
        return scan_res.has_x() && scan_res.has_o();
    }

    ScanResult scan_row(int i) const noexcept
    {
        ScanResult scan_res;
        for (int j = 0; j < board_size; ++j)
            scan_res.have(board[i][j]);
        return scan_res;
    }

    ScanResult scan_col(int j) const noexcept
    {
        ScanResult scan_res;
        for (int i = 0; i < board_size; ++i)
            scan_res.have(board[i][j]);
        return scan_res;
    }

    ScanResult scan_primary_diagonal() const noexcept
    {
        ScanResult scan_res;
        for (int i = 0; i < board_size; ++i)
            scan_res.have(board[i][i]);
        return scan_res;
    }

    ScanResult scan_secondary_diagonal() const noexcept
    {
        ScanResult scan_res;
        for (int i = 0; i < board_size; ++i)
            scan_res.have(board[i][board_size - 1 - i]);
        return scan_res;
    }

    void switch_player()
    {
        if (current_player == Player::X)
            current_player = Player::O;
        else
            current_player = Player::X;
    }

    Player current_player = Player::X;
    int nr_free_cells = board_size * board_size;
    Board<board_size> board {Player::None};
};

#ifndef CONFIG_SIZE
#define CONFIG_SIZE 3
#endif

constexpr int board_size = CONFIG_SIZE;

void reset_cursor()
{
    std::cout << "\033[" << board_size << "A";
}

void print_board(const Board<board_size>& board)
{
    for (int i = 0; i < board_size; ++i) {
        for (int j = 0; j < board_size; ++j) {
            char c = '.';
            switch (board[i][j]) {
            case Player::X:
                c = 'X';
                break;
            case Player::O:
                c = 'O';
                break;
            default:
                break;
            }
            std::cout << c << ' ';
        }
        std::cout << std::endl;
    }
}

std::pair<int, int> pick_random_free_cell(const Board<board_size>& board, int nr_free_cells)
{
    int free_cell_idx = rand() % nr_free_cells;
    int curr_free_cell_idx = 0;
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board.size(); ++j) {
            if (board[i][j] == Player::None && curr_free_cell_idx++ == free_cell_idx)
                return {i, j};
        }
    }
    return {-1, -1};
}

void print_usage(const char *name)
{
    std::cerr << "Usage: " << name << " <x_player_name> <o_player_name>\n";
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    seed();

    const char *x_player = argv[1];
    const char *o_player = argv[2];

    TicTacToe<board_size> game;
    print_board(game.get_board());

    while (true) {
        auto [i, j] = pick_random_free_cell(game.get_board(), game.get_nr_free_cells());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        game.take_turn(i, j);

        reset_cursor();
        print_board(game.get_board());

        auto [done, winner] = game.check();
        if (done) {
            if (winner != Player::O)
                std::cout << x_player;
            if (winner == Player::None)
                std::cout << ' ';
            if (winner != Player::X)
                std::cout << o_player;
            std::cout << std::endl;
            break;
        }
    }

    return EXIT_SUCCESS;
}
