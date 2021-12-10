#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>

std::optional<char> check_parenthes(std::string const& input)
{
    static const std::map<char, char> parentheses = {
        {'(', ')'},
        {'<', '>'},
        {'{', '}'},
        {'[', ']'},
    };

    std::deque<char> expected;

    for (auto const& c : input) {
        auto it = parentheses.find(c);
        if (it == parentheses.end()) {
            if (c == expected.back()) {
                expected.pop_back();
                continue;
            }
            return c;
        }
        expected.push_back(it->second);
    }
    return {};
}

int main()
{
    std::map<char, size_t> counts;

    std::string line;
    while (std::getline(std::cin, line)) {
        auto x = check_parenthes(line);
        if (!x) { continue; }
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

    return 0;
}
