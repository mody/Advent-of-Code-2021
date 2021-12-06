#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

/*
(int)(((7-offset)+days-left) / 7) = children

child n:
- days_left = (days - offset) + ((n-1) * 7)
*/

struct Fish {
    unsigned offset = 0;
};

using Pool = std::deque<Fish>;

void dump(Pool const& pool) {
    for (auto const& f : pool) {
        std::cout << f.offset << ",";
    }
    std::cout << " -> " << pool.size() << "\n";
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
            result.push_back({.offset = (unsigned)std::stoi(n)});
        }
        return result;
    }();

    for (unsigned day = 1; day < 80; ++day) {
        Pool children;
        for (Fish& f : pool) {
            if (--f.offset == 0) {
                children.push_back({.offset = 9});
                f.offset = 7;
            }
        }
        pool.insert(pool.end(), children.begin(), children.end());
    }
    std::cout << "1: " << pool.size() << "\n";

    return 0;
}
