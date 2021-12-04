#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

constexpr unsigned BOARD_SIZE = 5;

struct Board
{
    struct Number {
        unsigned n = 0;
        bool seen = false;
    };

    void add(unsigned number) {
        data.push_back({number, false});
    }

    bool draw(unsigned number) {
        for (auto& it : data) {
            if (it.n == number) {
                it.seen = true;
                break;
            }
        }
        for (unsigned line = 0; line < BOARD_SIZE; ++line) {
            if (data.at(line + 0).seen && data.at(line + 1).seen && data.at(line + 2).seen && data.at(line + 3).seen
                && data.at(line + 4).seen)
            {
                winning_number = number;
                finished = true;
                return true;
            }
        }
        for (unsigned col = 0; col < BOARD_SIZE; ++col) {
            if (data.at(col + BOARD_SIZE * 0).seen && data.at(col + BOARD_SIZE * 1).seen
                && data.at(col + BOARD_SIZE * 2).seen && data.at(col + BOARD_SIZE * 3).seen
                && data.at(col + BOARD_SIZE * 4).seen)
            {
                winning_number = number;
                finished = true;
                return true;
            }
        }
        return false;
    }

    unsigned score() const {
        assert(finished);

        unsigned unmarked = 0;
        for (auto const& it : data) {
            if (!it.seen) {
                unmarked += it.n;
            }
        }
        return unmarked * winning_number;
    }

    bool done() const
    {
        return finished;
    }

protected:
    std::vector<Number> data;
    unsigned winning_number = std::numeric_limits<unsigned>::max();
    bool finished = false;
};

using Boards = std::vector<Board>;


std::pair<unsigned, unsigned> first_last(std::vector<unsigned> const& randoms, Boards boards)
{
    std::vector<unsigned> winners;

    for (auto const& num : randoms) {
        if (boards.empty()) {
            break;
        }
        for (auto& board : boards) {
            board.draw(num);
        }
        auto it = std::stable_partition(boards.begin(), boards.end(), [](Board const& board) { return !board.done(); });
        if (it == boards.end()) {
            continue;
        }
        if (it == boards.begin()) {
            break;
        }
        for (auto x = it; x != boards.end(); ++x) {
            winners.push_back(x->score());
        }
        boards.erase(it, boards.end());
    }
    return {winners.front(), winners.back()};
}

int main()
{
    std::string line;
    std::getline(std::cin, line);
    assert(!line.empty());

    const std::vector<unsigned> randoms = [&line]() {
        std::vector<std::string> nums;
        boost::algorithm::split(nums, line, boost::algorithm::is_any_of(","));
        std::vector<unsigned> result;
        for (auto const& n : nums) {
            result.push_back(std::stoi(n));
        }
        return result;
    }();

    std::vector<Board> boards;

    while(std::getline(std::cin, line)) {
        assert(line.empty());

        Board b;

        for (unsigned i = 0; i < BOARD_SIZE * BOARD_SIZE; ++i) {
            unsigned n = std::numeric_limits<unsigned>::max();
            std::cin >> n;
            assert(std::cin);
            assert(n < std::numeric_limits<unsigned>::max());
            b.add(n);
        }
        (void)std::cin.get(); // eat the new-line

        boards.push_back(std::move(b));
    }

    auto const& [first, last] = first_last(randoms, boards);
    std::cout << "1: " << first << "\n2: " << last << "\n";

    return 0;
}
