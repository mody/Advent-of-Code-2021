#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <deque>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

constexpr unsigned DAYS1 = 80;
constexpr unsigned DAYS2 = 256;

struct Fish {
    int tick = 0;
    int days_left = DAYS2;
};

using Pool = std::deque<Fish>;

void part1(Pool pool) {
    for (unsigned day = 0; day < DAYS1; ++day) {
        Pool children;
        for (auto& f : pool) {
            if (f.tick == 0) {
                children.push_back({.tick = 8});
                f.tick = 7;
            }
        }
        for (auto& f : pool) {
            --f.tick;
        }
        pool.insert(pool.end(), children.begin(), children.end());
    }
    std::cout << "1: " << pool.size() << "\n";
}

uint64_t count_children(Fish const& f) {
    int children = f.days_left - f.tick;
    if (children > 0) {
        children = children / 7 + (int)(children % 7 != 0);
    } else {
        children = 0;
    }
    uint64_t result = children;
    for (int i = 0; i < children; ++i) {
        result += count_children({.tick = 9, .days_left = f.days_left - (f.tick + i * 7)});
    }
    return result;
}


void part2(Pool pool) {
    std::map<int, int> counts;
    for (auto const& f : pool) {
        counts.insert(std::make_pair(f.tick, 0U)).first->second += 1;
    }
    uint64_t result = 0;
    for (auto& [k, v] : counts) {
        result += v * count_children({.tick = k, .days_left = DAYS2}) + 1;
    }
}

int main()
{
    std::string line;
    std::getline(std::cin, line);
    assert(!line.empty());

    Pool pool = [&line]() {
        std::vector<std::string> nums;
        boost::algorithm::split(nums, line, boost::algorithm::is_any_of(","));
        Pool result;
        for (auto const& n : nums) {
            result.push_back({.tick = std::stoi(n)});
        }
        return result;
    }();

    part1(pool);
    part2(pool);

    return 0;
}
