#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::vector<std::string> input;

    std::string line;
    while(std::getline(std::cin, line)) {
        assert(!line.empty());
        input.push_back(std::move(line));
    }

    assert(!input.empty());

    std::string most, least;

    for (size_t i = 0; i < input.at(0).size(); ++i) {
        int zeros = 0, ones = 0;
        for (auto const& num : input) {
            if (num.at(i) == '1') {
                ++ones;
            } else {
                ++zeros;
            }
        }
        if (ones > zeros) {
            most.append("1");
            least.append("0");
        } else {
            most.append("0");
            least.append("1");
        }
    }

    auto gamma = stoll(most, 0, 2);
    auto epsilon = stoll(least, 0, 2);

    std::cout << "gamma: " << gamma << ", epsilon: " << epsilon << "\n1: " << gamma*epsilon << "\n";

    return 0;
}
