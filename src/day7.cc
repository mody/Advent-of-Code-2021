#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <cmath>
#include <deque>
#include <iomanip>
#include <iostream>
#include <numeric>
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

void part2(Pool pool) {
    long sum = std::accumulate(pool.begin(), pool.end(), 0, [](long const& a, long const& b) { return a + b; });

    long mean = sum / pool.size();
    long fuel = 0;
    for (auto const& i : pool) {
        for (long x = 1; x <= std::abs(mean - i); ++x) {
            fuel += x;
        }
    }

    std::cout << "2: median: " << mean << ", fuel: " << fuel << "\n";
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
    part2(pool);

    return 0;
}
