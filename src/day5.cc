#include <boost/container_hash/hash.hpp>
#include <cassert>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

struct Point
{
    int64_t x = 0, y = 0;

    constexpr Point(int64_t x_, int64_t y_) noexcept
        : x {x_}
        , y {y_}
    { }

    bool operator==(Point const& o) const noexcept
    {
        return x == o.x && y == o.y;
    }

    Point& operator+=(Point const& o) noexcept
    {
        x += o.x;
        y += o.y;
        return *this;
    }
};

namespace std {

template<>
struct hash<Point>
{
    size_t operator()(Point const& p) const noexcept
    {
        size_t seed = 0;
        boost::hash_combine(seed, p.x);
        boost::hash_combine(seed, p.y);
        return seed;
    }
};

std::ostream& operator<<(std::ostream& os, Point const& p)
{
    os << "(" << p.x << "; " << p.y << ")";
    return os;
}

}  // namespace std

using Mapa = std::unordered_map<Point, std::pair<unsigned, unsigned>>;


int main()
{
    static const std::regex INPUT_RX{"^(\\d+),(\\d+) -> (\\d+),(\\d+)$"};

    Mapa mapa;

    std::string line;
    while(std::getline(std::cin, line)) {
        std::smatch m;
        if (!std::regex_match(line, m, INPUT_RX)) {
            std::cout << "ERROR! Unknown command! [" << line << "]\n";
            exit(1);
        }
        Point p1(std::stoul(m.str(1)), std::stoul(m.str(2)));
        Point p2(std::stoul(m.str(3)), std::stoul(m.str(4)));
        Point d(0, 0);
        if (p1.x == p2.x) {
            d.y = (p1.y < p2.y) ? 1 : -1;
        } else if (p1.y == p2.y) {
            d.x = (p1.x < p2.x) ? 1 : -1;
        } else {
            d.x = (p1.x < p2.x) ? 1 : -1;
            d.y = (p1.y < p2.y) ? 1 : -1;
        }

        auto update_map = [&](Point const& px) {
            auto& data = mapa.insert({px, {0, 0}}).first->second;
            if (d.x == 0 || d.y == 0) {
                data.first += 1;
            } else {
                data.second += 1;
            }
        };

        for (; !(p1 == p2); p1 += d)
        {
            update_map(p1);
        }
        update_map(p1);
    }

    unsigned result1 = 0;
    for (auto const& [px, cnt] : mapa) {
        if (cnt.first > 1) {
            ++result1;
        }
    }
    std::cout << "1: " << result1 << "\n";

    unsigned result2 = 0;
    for (auto const& [px, cnt] : mapa) {
        if (cnt.first + cnt.second > 1) {
            ++result2;
        }
    }
    std::cout << "2: " << result2 << "\n";

    return 0;
}
