#include <boost/container_hash/hash.hpp>
#include <cassert>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

static std::regex RX {"^fold along ([a-z])=(\\d+)$"};

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

using Mapa = std::unordered_map<Point, unsigned>;

std::pair<int64_t, int64_t> measure(Mapa const& mapa)
{
    int64_t max_x = std::numeric_limits<int64_t>::min();
    int64_t max_y = std::numeric_limits<int64_t>::min();
    for (auto const& [p, _] : mapa) {
        max_x = std::max(max_x, p.x);
        max_y = std::max(max_y, p.y);
    }
    ++max_x;
    ++max_y;
    return {max_x, max_y};
}


void dump(Mapa const& mapa)
{
    const auto [max_x, max_y] = measure(mapa);

    for (int64_t y = 0; y < max_y; ++y) {
        for (int64_t x = 0; x < max_x; ++x) {
            auto it = mapa.find({x, y});
            if (it == mapa.end()) {
                std::cout << '.';
            } else {
                std::cout << '#';
            }
        }
        std::cout << "\n";
    }

}

void fold(Mapa& mapa, int64_t by_x, int64_t by_y)
{
    std::function<Point(Point const&)> op;

    if (by_x) {
        assert(by_y == 0);
        op = [&](Point const& p) -> Point {
            if (p.x > by_x) {
                return {by_x - (p.x - by_x), p.y};
            }
            return p;
        };
    } else {
        assert(by_x == 0);
        op = [&](Point const& p) -> Point {
            if (p.y > by_y) {
                return {p.x, by_y - (p.y - by_y)};
            }
            return p;
        };
    }

    Mapa m2;
    for (auto p : mapa) {
        m2.insert({op(p.first), p.second});
    }
    std::swap(m2, mapa);
}


int main()
{
    Mapa mapa;

    std::string line;
    while(std::getline(std::cin, line))
    {
        if (line.empty()) {
            break;
        }
        auto sep = line.find(',');
        assert(sep != std::string::npos);
        mapa.insert({{std::stoll(line.substr(0, sep)), std::stoll(line.substr(sep + 1))}, 0}).first->second++;
    }

    // dump(mapa);

    unsigned folds = 0;
    while(std::getline(std::cin, line)) {
        // std::cout << line << "\n";
        if (line.empty()) break;
        std::smatch m;
        if (!std::regex_match(line, m, RX)) {
            std::cout << "ERROR on line " << line << "\n";
            exit(1);
        }
        int64_t by_x = 0, by_y = 0;
        if (m.str(1) == "x") {
            by_x = std::stoll(m.str(2));
        } else {
            by_y = std::stoll(m.str(2));
        }

        fold(mapa, by_x, by_y);
        if (folds == 0) {
            std::cout << "1: " << mapa.size() << "\n";
        }
        // dump(mapa);
        ++folds;
    }

    return 0;
}
