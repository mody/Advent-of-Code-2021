#include <boost/container_hash/hash.hpp>

#include <cassert>
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

struct Point {
    int64_t x = 0, y = 0, z = 0;

    bool operator==(Point const& o) const noexcept { return x == o.x && y == o.y && z == o.z; }
};

Point operator+(Point lhs, Point const& rhs) noexcept {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    lhs.z += rhs.z;
    return lhs;
}

namespace std {

template <>
struct hash<Point>
{
    size_t operator()(Point const& p) const noexcept
    {
        size_t seed = 0;
        boost::hash_combine(seed, p.x);
        boost::hash_combine(seed, p.y);
        boost::hash_combine(seed, p.z);
        return seed;
    }
};

}  // namespace std


struct World
{
    World(int64_t l)
        : limit {l}
    { }

    void insert(
        bool on_off, int64_t _from_x, int64_t _to_x, int64_t _from_y, int64_t _to_y, int64_t _from_z, int64_t _to_z)
    {
        if (limit) {
            int64_t from_x = std::max(-limit, _from_x);
            int64_t to_x = std::min(limit, _to_x);
            int64_t from_y = std::max(-limit, _from_y);
            int64_t to_y = std::min(limit, _to_y);
            int64_t from_z = std::max(-limit, _from_z);
            int64_t to_z = std::min(limit, _to_z);
            insert_impl(on_off, from_x, to_x, from_y, to_y, from_z, to_z);
        } else {
            insert_impl(on_off, _from_x, _to_x, _from_y, _to_y, _from_z, _to_z);
        }
    }

    size_t size() const
    {
        return world.size();
    }

protected:
    void insert_impl(
        bool on_off, int64_t from_x, int64_t to_x, int64_t from_y, int64_t to_y, int64_t from_z, int64_t to_z)
    {
        for (int64_t x = from_x; x <= to_x; ++x) {
            for (int64_t y = from_y; y <= to_y; ++y) {
                for (int64_t z = from_z; z <= to_z; ++z) {
                    const size_t h = std::hash<Point>()({x,y,z});
                    if (on_off) {
                        world.insert(h);
                    } else {
                        world.erase(h);
                    }
                }
            }
        }
    }

    int64_t limit = 0;
    std::unordered_set<size_t> world;
};

int main(int argc, char* argv[]) {
    // on x=10..12,y=10..12,z=10..12
    static const std::regex RX{"^(on|off) x=([0-9-]+)\\.\\.([0-9-]+),y=([0-9-]+)\\.\\.([0-9-]+),z=([0-9-]+)\\.\\.([0-9-]+)$"};

    World world50(50);
    World world(0);

    int64_t min_x = std::numeric_limits<int64_t>::max();
    int64_t min_y = std::numeric_limits<int64_t>::max();
    int64_t min_z = std::numeric_limits<int64_t>::max();
    int64_t max_x = std::numeric_limits<int64_t>::min();
    int64_t max_y = std::numeric_limits<int64_t>::min();
    int64_t max_z = std::numeric_limits<int64_t>::min();

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        std::smatch m;
        if (!std::regex_match(line, m, RX)) {
            std::cerr << "Error parsing >" << line << "<\n";
            exit(1);
        }
        assert(m.size() == 8);

        bool on_off = m.str(1) == "on";
        int64_t from_x = std::stoll(m.str(2));
        int64_t to_x = std::stoll(m.str(3));
        int64_t from_y = std::stoll(m.str(4));
        int64_t to_y = std::stoll(m.str(5));
        int64_t from_z = std::stoll(m.str(6));
        int64_t to_z = std::stoll(m.str(7));

        min_x = std::min(min_x, from_x);
        min_y = std::min(min_y, from_y);
        min_z = std::min(min_z, from_z);

        max_x = std::max(max_x, to_x);
        max_y = std::max(max_y, to_y);
        max_z = std::max(max_z, to_z);

        world50.insert(on_off, from_x, to_x, from_y, to_y, from_z, to_z);
        // world.insert(on_off, from_x, to_x, from_y, to_y, from_z, to_z);
    }

    std::cout << min_x << ".." << max_x << " = " << (max_x - min_x + 1) << "\n";
    std::cout << min_y << ".." << max_y << " = " << (max_y - min_y + 1) << "\n";
    std::cout << min_z << ".." << max_z << " = " << (max_z - min_z + 1) << "\n";

    std::cout << "1 Result=" << world50.size() << "\n";
    std::cout << "2 Result=" << world.size() << "\n";

    return 0;
}
