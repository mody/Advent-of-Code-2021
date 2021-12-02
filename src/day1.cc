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
    std::deque<unsigned> windowA, windowB;

    auto itA = measurement.begin();
    auto itB = std::next(itA);

    for (; itB != measurement.end(); ++itA, ++itB) {
        windowA.push_back(*itA);
        windowB.push_back(*itB);

        if (windowA.size() > 3) {
            windowA.pop_front();
        }
        if (windowB.size() > 3) {
            windowB.pop_front();
        }

        if (windowA.size() == 3 && windowB.size() == 3) {
            if ((windowA.at(0) + windowA.at(1) + windowA.at(2)) < (windowB.at(0) + windowB.at(1) + windowB.at(2))) {
                ++result;
            }
        }
    }

    std::cout << "2: " << result << "\n ";

    return 0;
}
