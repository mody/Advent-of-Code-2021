#include <boost/container_hash/hash.hpp>

#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>

struct Dice {
    Dice(uint64_t _max_val)
        : MAX_VALUE {_max_val}
    { }

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

protected:
    const uint64_t MAX_VALUE;
    uint64_t value = 0;
    uint64_t rolls_count = 0;
};

void part1(uint64_t p1, uint64_t p2)
{
    static constexpr uint64_t BOARD_SIZE = 10;
    uint64_t score1 = 0, score2 = 0;
    Dice d{100};

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
}

struct Player {
    Player(uint64_t starting_pos)
        : pos {starting_pos}
    { }

    void move(unsigned delta) {
        pos += delta % BOARD_SIZE;
        if (pos > BOARD_SIZE) {
            pos -= BOARD_SIZE;
        }
        score += pos;
    }

    bool operator==(Player const& o) const noexcept
    {
        return pos == o.pos && score == o.score;
    }

    uint64_t pos = 0;
    uint64_t score = 0;
    static constexpr uint64_t BOARD_SIZE = 10;
};

namespace std {

template<>
struct hash<Player>
{
    size_t operator()(Player const& p) const noexcept
    {
        size_t seed = 0;
        boost::hash_combine(seed, p.pos);
        boost::hash_combine(seed, p.score);
        return seed;
    }
};

template<>
struct hash<std::pair<Player, Player>>
{
    size_t operator()(std::pair<Player, Player> const& p) const noexcept
    {
        size_t seed = 0;
        boost::hash_combine(seed, p.first.pos);
        boost::hash_combine(seed, p.first.score);
        boost::hash_combine(seed, p.second.pos);
        boost::hash_combine(seed, p.second.score);
        return seed;
    }
};

std::ostream& operator<<(std::ostream& os, Player const& p)
{
    os << "(" << p.pos << "; " << p.score << ")";
    return os;
}

}  // namespace std

using WorldMap = std::unordered_map<std::pair<Player, Player>, std::pair<uint64_t, uint64_t>>;


std::pair<uint64_t, uint64_t> process(WorldMap& w, Player p1, Player p2)
{
    if (p1.score >= 21) {
        return {1, 0};
    }
    if (p2.score >= 21) {
        return {0, 1};
    }
    auto it = w.find({p1, p2});
    if (it != w.end()) {
        return it->second;
    }

    std::pair<uint64_t, uint64_t> winnings;

    for (unsigned i = 1; i <= 3; ++i) {
        for (unsigned ii = 1; ii <= 3; ++ii) {
            for (unsigned iii = 1; iii <= 3; ++iii) {
                Player p = p1;
                p.move(i + ii + iii);
                auto x = process(w, p2, p);
                winnings.first += x.second;
                winnings.second += x.first;
            }
        }
    }

    w[{p1, p2}] = winnings;
    return winnings;
}

void part2(uint64_t pos1, uint64_t pos2)
{
    WorldMap w;

    auto r = process(w, pos1, pos2);
    std::cout << "wins: " << std::max(r.first, r.second) << "\n";
}

int main()
{
    uint64_t p1 = 0, p2 = 0;

    std::string line;
    std::getline(std::cin, line);
    assert(!line.empty());
    p1 = std::stoi(line.substr(line.size()-1));

    std::getline(std::cin, line);
    assert(!line.empty());
    p2 = std::stoi(line.substr(line.size()-1));

    std::cout << "Player 1 starting position: " << p1 << "\n";
    std::cout << "Player 2 starting position: " << p2 << "\n";

    part1(p1, p2);
    part2(p1, p2);

    // part2 = 446968027750017

    return 0;
}
