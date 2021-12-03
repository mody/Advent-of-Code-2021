#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using Input = std::vector<std::string>;
using MostLeast = std::pair<std::string, std::string>;

MostLeast most_least(Input const& input)
{
    std::string most, least;
    most.resize(input.at(0).size());
    least.resize(input.at(0).size());

    for (size_t i = 0; i < input.at(0).size(); ++i) {
        int zeros = 0, ones = 0;
        for (auto const& num : input) {
            if (num.at(i) == '1') {
                ++ones;
            } else {
                ++zeros;
            }
        }
        if (ones >= zeros) {
            most.at(i) = '1';
            least.at(i) = '0';
        } else {
            most.at(i) = '0';
            least.at(i) = '1';
        }
    }
    return {most, least};
}

template<typename OP>
size_t filter_input(Input input, OP&& op)
{
    assert(!input.empty());

    for (size_t i = 0; i < input.at(0).size(); ++i) {
        std::string const& bits = op(most_least(input));
        const auto c = bits.at(i);

        auto end = std::remove_if(input.begin(), input.end(), [&](std::string const& line) { return line.at(i) != c; });
        input.erase(end, input.end());

        if (input.size() == 1) {
            break;
        }
    }

    assert(input.size() == 1);
    return std::stoul(input.at(0), 0, 2);
}

int main()
{
    Input input;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        input.push_back(std::move(line));
    }

    assert(!input.empty());

    auto const& [most, least] = most_least(input);

    auto gamma = stoll(most, 0, 2);
    auto epsilon = stoll(least, 0, 2);

    std::cout << "gamma: " << gamma << ", epsilon: " << epsilon << "\n1: " << gamma*epsilon << "\n";

    auto oxygen = filter_input(input, [](MostLeast const& ml) { return ml.first; });
    auto co2 = filter_input(input, [](MostLeast const& ml) { return ml.second; });

    std::cout << "oxygen: " << oxygen << ", CO2: " << co2 << "\n2: " << oxygen*co2 << "\n";

    return 0;
}
