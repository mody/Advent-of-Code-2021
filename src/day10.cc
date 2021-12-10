#include <algorithm>
#include <deque>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

static const std::map<char, char> PARENTHESES = {
    {'(', ')'},
    {'<', '>'},
    {'{', '}'},
    {'[', ']'},
};

std::pair<std::optional<char>, std::string> check_parenthes(std::string const& input)
{
    std::deque<char> expected;

    for (auto const& c : input) {
        auto it = PARENTHESES.find(c);
        if (it == PARENTHESES.end()) {
            if (c == expected.front()) {
                expected.pop_front();
                continue;
            }
            return {c, {}};
        }
        expected.push_front(it->second);
    }
    std::string missing;
    for (auto const& c : expected) {
        missing += c;
    }
    return std::make_pair(std::nullopt, missing);
}

int main()
{
    std::map<char, size_t> counts;
    std::vector<std::string> missing;

    std::string line;
    while (std::getline(std::cin, line)) {
        auto const& [x, _missing] = check_parenthes(line);
        if (!x) {
            missing.push_back(_missing);
            continue;
        }
        counts.insert({*x, 0}).first->second++;
    }

    uint64_t result1 = 0;
    for (auto const& [k, v] : counts) {
        switch (k) {
        case ')': result1 += v * 3; break;
        case ']': result1 += v * 57; break;
        case '}': result1 += v * 1197; break;
        case '>': result1 += v * 25137; break;
        }
    }

    std::cout << "1: " << result1 << "\n";

    std::vector<uint64_t> counts2;
    for (auto const& line : missing) {
        uint64_t result2 = 0;
        for (auto const& c : line) {
            result2 *= 5;
            switch (c) {
            case ')': result2 += 1; break;
            case ']': result2 += 2; break;
            case '}': result2 += 3; break;
            case '>': result2 += 4; break;
            }
        }
        counts2.push_back(result2);
    }
    std::sort(counts2.begin(), counts2.end());

    std::cout << "2: " << counts2.at(counts2.size() / 2) << "\n";

    return 0;
}
