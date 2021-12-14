#include <cassert>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <utility>

static std::regex RX {"^([A-Z]+) -> ([A-Z])$"};

using Rules = std::map<std::string, std::string>;

int main()
{
    std::string polymer;
    std::getline(std::cin, polymer);
    assert(!polymer.empty());

    Rules rules;

    std::string line;
    std::getline(std::cin, line);
    assert(line.empty());

    while(std::getline(std::cin, line))
    {
        if (line.empty()) {
            break;
        }
        std::smatch m;
        if (!std::regex_match(line, m, RX)) {
            std::cout << "ERROR on line " << line << "\n";
            exit(1);
        }
        assert(m.str(1).size() == 2);
        assert(m.str(2).size() == 1);
        rules[m.str(1)] = m.str(2);
    }

    for (int i = 0; i < 10; ++i) {
        std::string p2;
        p2.reserve(polymer.size() * 2);
        char last = 0;
        for (auto c : polymer) {
            if (last == 0) {
                last = c;
                continue;
            }
            auto it = rules.find(std::string {last} + c);
            assert(it != rules.end());
            p2 += last + it->second;
            last = c;
        }
        p2 += last;
        std::swap(p2, polymer);
    }

    std::map<char, size_t> counts;
    for (auto c : polymer) {
        counts.insert({c, 0}).first->second++;
    }
    std::vector<std::pair<char, size_t>> data;
    for (auto p : counts) {
        data.push_back(std::move(p));
    }
    std::sort(data.begin(), data.end(), [](std::pair<char, size_t> const& a, std::pair<char, size_t> const& b) {
        return a.second < b.second;
    });
    std::cout << (data.back().second - data.front().second) << "\n";

    return 0;
}
