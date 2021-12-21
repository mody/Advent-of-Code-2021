#include <cassert>
#include <iostream>
#include <string>

struct Dice {
    uint64_t roll() noexcept {
        ++rolls_count;
        ++value;
        if (value > MAX_VALUE) {
            value = 1;
        }
        return value;
    }

    uint64_t rolls() const noexcept
    {
        return rolls_count;
    }

    static constexpr uint64_t MAX_VALUE = 100;
protected:
    uint64_t value = 0;
    uint64_t rolls_count = 0;
};


int main()
{
    Dice d;

    uint64_t p1 = 0, p2 = 0;
    uint64_t score1 = 0, score2 = 0;

    std::string line;
    std::getline(std::cin, line);
    assert(!line.empty());
    p1 = std::stoi(line.substr(line.size()-1));

    std::getline(std::cin, line);
    assert(!line.empty());
    p2 = std::stoi(line.substr(line.size()-1));

    // std::cout << "Player 1 starting position: " << p1 << "\n";
    // std::cout << "Player 2 starting position: " << p2 << "\n";

    static constexpr uint64_t BOARD_SIZE = 10;

    for (;;) {
        uint64_t skip = d.roll() + d.roll() + d.roll();
        p1 += skip % BOARD_SIZE;
        if (p1 > BOARD_SIZE) {
            p1 -= BOARD_SIZE;
        }
        score1 += p1;
        // std::cout << "P1 pos: " << p1 << ", score: " << score1 << "\n";

        if (score1 >= 1000) { break; }

        skip = d.roll() + d.roll() + d.roll();
        p2 += skip % BOARD_SIZE;
        if (p2 > BOARD_SIZE) {
            p2 -= BOARD_SIZE;
        }
        score2 += p2;
        // std::cout << "P2 pos: " << p2 << ", score: " << score2 << "\n";

        if (score2 >= 1000) { break; }
    }
    // std::cout << "rolls: " << d.rolls() << "\n";
    std::cout << "1: " << d.rolls() * std::min(score1, score2) << "\n";

    return 0;
}
