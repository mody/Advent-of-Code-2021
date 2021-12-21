#include <boost/container_hash/hash.hpp>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

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

    // Point& operator+=(Point const& o) noexcept
    // {
    //     x += o.x;
    //     y += o.y;
    //     return *this;
    // }
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

struct MapSize {
    int64_t min_x = std::numeric_limits<int64_t>::max();
    int64_t min_y = std::numeric_limits<int64_t>::max();

    int64_t max_x = std::numeric_limits<int64_t>::min();
    int64_t max_y = std::numeric_limits<int64_t>::min();
};

using Mapa = std::unordered_map<Point, char>;
using Mask = std::vector<bool>;

MapSize measure(Mapa const& mapa)
{
    MapSize ms;
    for (auto const& [p, _] : mapa) {
        ms.min_x = std::min(ms.min_x, p.x);
        ms.min_y = std::min(ms.min_y, p.y);
        ms.max_x = std::max(ms.max_x, p.x);
        ms.max_y = std::max(ms.max_y, p.y);
    }
    ++ms.max_x;
    ++ms.max_y;
    return ms;
}


void dump(Mapa const& mapa)
{
    auto const& ms = measure(mapa);

    for (int64_t y = ms.min_y; y < ms.max_y; ++y) {
        for (int64_t x = ms.min_x; x < ms.max_x; ++x) {
            auto it = mapa.find({x, y});
            if (it == mapa.end()) {
                std::cout << ' ';
            } else {
                std::cout << it->second;
            }
        }
        std::cout << "\n";
    }

}


void part1(Mapa mapa, Mask const& mask)
{
    // dump(mapa);

    unsigned void_value = mask.at(0) ? 1 : 0;

    for (int i = 0 ; i < 2; ++i) {
        auto const& ms = measure(mapa);
        Mapa m2;

        auto point_to_bit = [&](int64_t x, int64_t y) -> unsigned {
            auto it = mapa.find({x, y});
            if (it != mapa.end()) {
                return it->second == '#' ? 1 : 0;
            }
            if (void_value) {
                return mask.at(mask.size() - 1) ? 1 : 0;
            }
            return mask.at(0) ? 1 : 0;
        };

        for (int64_t y = ms.min_y - 2; y < ms.max_y + 2; ++y) {
            for (int64_t x = ms.min_x - 2; x < ms.max_x + 2; ++x) {
                uint64_t bits = 0;
                bits |= point_to_bit(x - 1, y - 1);
                bits <<= 1;
                bits |= point_to_bit(x    , y - 1);
                bits <<= 1;
                bits |= point_to_bit(x + 1, y - 1);
                bits <<= 1;
                bits |= point_to_bit(x - 1, y);
                bits <<= 1;
                bits |= point_to_bit(x    , y);
                bits <<= 1;
                bits |= point_to_bit(x + 1, y);
                bits <<= 1;
                bits |= point_to_bit(x - 1, y + 1);
                bits <<= 1;
                bits |= point_to_bit(x    , y + 1);
                bits <<= 1;
                bits |= point_to_bit(x + 1, y + 1);

                assert(bits < mask.size());
                m2.insert({{x, y}, mask.at(bits) ? '#' : '.'});
            }
        }
        std::swap(mapa, m2);
        // std::cout << "\nIteration: " << i << "\n";
        // dump(mapa);

        if (void_value) {
            void_value = mask.at(mask.size() - 1) ? 1 : 0;
        } else {
            void_value = mask.at(0) ? 1 : 0;
        }
    }

    unsigned count = 0;
    for (auto const& [_, c] : mapa) {
        if (c == '#') {
            ++count;
        }
    }
    std::cout << "1: " << count << "\n";
}

/*
Answer (part 1): 5786
Answer (part 2): 16757
 */

int main()
{
    Mapa mapa;
    Mask mask;

    std::string line;
    std::getline(std::cin, line);
    assert(!line.empty());
    for(auto const& c : line) {
        mask.push_back(c == '#');
    }

    std::getline(std::cin, line);
    assert(line.empty());

    int x = 0, y = 0;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        for (auto const& c : line) {
            mapa.insert({{x, y}, c});
            ++x;
        }
        ++y;
        x = 0;
    }

    part1(mapa, mask);

    return 0;
}
