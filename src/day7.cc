#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cmath>
#include <deque>
#include <iomanip>
#include <iostream>
#include <string>

using Pool = std::deque<long>;

void part1(Pool pool) {
    std::sort(pool.begin(), pool.end());

    long median = pool.at(pool.size()/2);
    long fuel = 0;

    for (auto const& i : pool) {
        fuel += std::abs(median - i);
    }

    std::cout << "1: median: " << median << ", fuel: " << fuel << "\n";
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
            result.push_back(std::stoi(n));
        }
        return result;
    }();

    part1(pool);

    return 0;
}
