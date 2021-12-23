#include <algorithm>
#include <boost/container_hash/hash.hpp>

#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <unordered_set>
#include <vector>


struct Step {
    bool on_off = false;
    int64_t from_x = 0;
    int64_t to_x = 0;
    int64_t from_y = 0;
    int64_t to_y = 0;
    int64_t from_z = 0;
    int64_t to_z = 0;
};

namespace std {

std::ostream& operator<<(std::ostream& os, Step const& s)
{
    os << (s.on_off ? "on" : "off") << " x=" << s.from_x << ".." << s.to_x << ",y=" << s.from_y << ".." << s.to_y
       << ",z=" << s.from_z << ".." << s.to_z << "\n";
    return os;
}

}  // namespace std


using Steps = std::vector<Step>;

uint64_t process(Steps const& steps)
{
    std::vector<int64_t> X, Y, Z;

    auto coord_to_index = [](std::vector<int64_t> const& v, int64_t coord) -> size_t {
        auto it = std::lower_bound(v.begin(), v.end(), coord);
        return std::distance(v.begin(), it);
    };

    for (auto const& s : steps) {
        X.push_back(s.from_x);
        X.push_back(s.to_x);
        Y.push_back(s.from_y);
        Y.push_back(s.to_y);
        Z.push_back(s.from_z);
        Z.push_back(s.to_z);
    }
    std::sort(X.begin(), X.end());
    std::sort(Y.begin(), Y.end());
    std::sort(Z.begin(), Z.end());

    const auto N = X.size();
    std::vector<std::vector<std::vector<bool>>> world(
        N, std::vector<std::vector<bool>>(N, std::vector<bool>(N, false)));

    for (auto const& s : steps) {
        const auto idx_x = coord_to_index(X, s.from_x);
        const auto idx_y = coord_to_index(Y, s.from_y);
        const auto idx_z = coord_to_index(Z, s.from_z);

        const auto to_x = coord_to_index(X, s.to_x);
        const auto to_y = coord_to_index(Y, s.to_y);
        const auto to_z = coord_to_index(Z, s.to_z);

        for (auto x = idx_x; x < to_x; ++x) {
            for (auto y = idx_y; y < to_y; ++y) {
                for (auto z = idx_z; z < to_z; ++z) {
                    world[x][y][z] = s.on_off;
                }
            }
        }
    }

    uint64_t count = 0;
    for (size_t x = 0; x < N; ++x) {
        for (size_t y = 0; y < N; ++y) {
            for (size_t z = 0; z < N; ++z) {
                if (world[x][y][z]) {
                    count += (X.at(x + 1) - X.at(x)) * (Y.at(y + 1) - Y.at(y)) * (Z.at(z + 1) - Z.at(z));
                }
                // count += (world[x][y][z]) * (X[x + 1] - X[x]) * (Y[y + 1] - Y[y]) * (Z[z + 1] - Z[z]);
            }
        }
    }

    return count;
}


void part1(Steps steps) {
    auto limitter50 = [](int64_t val) -> int64_t {
        if (val < -50) {
            return -50;
        }
        if (val > 51) {
            return 51;
        }
        return val;
    };

    for (auto& s : steps) {
        s.from_x = limitter50(s.from_x);
        s.from_y = limitter50(s.from_y);
        s.from_z = limitter50(s.from_z);

        s.to_x = limitter50(s.to_x);
        s.to_y = limitter50(s.to_y);
        s.to_z = limitter50(s.to_z);
    }

    auto r = process(steps);
    std::cout << "1: " << r << "\n";
}


void part2(Steps steps) {
    auto r = process(steps);
    std::cout << "2: " << r << "\n";
}

int main(int argc, char* argv[]) {
    static const std::regex RX{"^(on|off) x=([0-9-]+)\\.\\.([0-9-]+),y=([0-9-]+)\\.\\.([0-9-]+),z=([0-9-]+)\\.\\.([0-9-]+)$"};

    Steps steps;

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

        steps.emplace_back();
        auto& step = steps.back();

        step.on_off = m.str(1) == "on";
        step.from_x = std::stoll(m.str(2));
        step.to_x = std::stoll(m.str(3)) + 1;
        step.from_y = std::stoll(m.str(4));
        step.to_y = std::stoll(m.str(5)) + 1;
        step.from_z = std::stoll(m.str(6));
        step.to_z = std::stoll(m.str(7)) + 1;
    }

    part1(steps);
    part2(steps);

    return 0;
}
