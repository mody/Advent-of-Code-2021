#include <deque>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::vector<unsigned> measurement;

    unsigned result = 0;
    unsigned last = 0;
    std::string line;
    while(std::cin) {
        unsigned depth;
        std::cin >> depth;
        measurement.push_back(depth);
        if (last) {
            if (depth > last) {
                ++result;
            }
        }
        last = depth;
    }

    std::cout << "1: " << result << "\n";

    result = 0;
    last = 0;
    std::deque<unsigned> window;

    for (auto const& i : measurement) {
        window.push_back(i);
        if (window.size() < 3) {
            continue;
        }

        if (last == 0) {
            last = window.at(0) + window.at(1) + window.at(2);
            continue;
        }

        if (window.size() > 3) {
            window.pop_front();
        }

        const auto current = window.at(0) + window.at(1) + window.at(2);
        if (last < current) {
            ++result;
        }

        last = current;
    }

    std::cout << "2: " << result << "\n ";

    return 0;
}
