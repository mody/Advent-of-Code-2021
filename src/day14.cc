#include <cassert>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

static std::regex RX {"^([A-Z]+) -> ([A-Z])$"};

using Rules = std::unordered_map<std::string, std::string>;

uint64_t compute(std::string polymer, Rules const& rules, int iterations) {
    std::unordered_map<std::string, uint64_t> counts;

    char last = 0;
    for (auto c : polymer) {
        if (last != 0) {
            counts.insert({std::string {last} + c, 0}).first->second++;
        }
        last = c;
    }

    for (int i = 0; i < iterations; ++i) {
        std::unordered_map<std::string, uint64_t> c2;
        for (auto const& [k, v] : counts) {
            auto it = rules.find(k);
            if (it == rules.end()) {
                c2.insert({k, 0}).first->second += v;
                continue;
            }
            std::string k1 {k.at(0) + it->second};
            std::string k2 {it->second + k.at(1)};
            c2.insert({k1, 0}).first->second += v;
            c2.insert({k2, 0}).first->second += v;
        }
        std::swap(counts, c2);
    }


    std::unordered_map<char, size_t> c3;
    for (auto const& [k, v] : counts) {
        c3.insert({k.at(0), 0}).first->second += v;
    }
    c3.insert({polymer.back(), 0}).first->second++;
    std::vector<std::pair<char, uint64_t>> data;
    for (auto const& [k, v] : c3) {
        data.push_back({k, v});
    }
    std::sort(data.begin(), data.end(), [](std::pair<char, size_t> const& a, std::pair<char, size_t> const& b) {
        return a.second < b.second;
    });
    return (data.back().second - data.front().second);
}


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

    std::cout << "1: " << compute(polymer, rules, 10) << "\n";
    std::cout << "2: " << compute(polymer, rules, 40) << "\n";

    return 0;
}
