#include <algorithm>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using Words = std::vector<std::string>;
using WordList = std::deque<Words>;

/*

 aaaa
f    b
f    b
 gggg
e    c
e    c
 dddd

 aaaa    ....    aaaa    aaaa    ....
f    b  .    b  .    b  .    b  f    b
f    b  .    b  .    b  .    b  f    b
 ....    ....    gggg    gggg    gggg
e    c  .    c  e    .  .    c  .    c
e    c  .    c  e    .  .    c  .    c
 dddd    ....    dddd    dddd    ....

 aaaa    aaaa    aaaa    aaaa    aaaa
f    .  f    .  .    b  f    b  f    b
f    .  f    .  .    b  f    b  f    b
 gggg    gggg    ....    gggg    gggg
.    c  e    c  .    c  e    c  .    c
.    c  e    c  .    c  e    c  .    c
 dddd    dddd    ....    dddd    dddd

*/

uint64_t decode(Words const& words)
{
    std::map<std::string, int> TO_NUMBERS;
    std::map<int, std::string> FROM_NUMBERS;

    for (auto const& w : words) {
        if (w.size() == 2) {
            TO_NUMBERS[w] = 1;
            FROM_NUMBERS[1] = w;
        } else if (w.size() == 3) {
            TO_NUMBERS[w] = 7;
            FROM_NUMBERS[7] = w;
        } else if (w.size() == 4) {
            TO_NUMBERS[w] = 4;
            FROM_NUMBERS[4] = w;
        } else if (w.size() == 7) {
            TO_NUMBERS[w] = 8;
            FROM_NUMBERS[8] = w;
        }
    }

    auto merge = [](std::string a, std::string const& b) -> std::string {
        for (auto c : b) {
            a += c;
        }
        std::sort(a.begin(), a.end());
        a.erase(std::unique(a.begin(), a.end()), a.end());
        assert(a.size() < 8);
        return a;
    };

    for (auto const& w : words) {
        if (w.size() == 6) {
            const auto x = merge(w, FROM_NUMBERS.at(4));
            if (x.size() == 6) {
                TO_NUMBERS[w] = 9;
                FROM_NUMBERS[9] = w;
                continue;
            }
            auto const& one = FROM_NUMBERS.at(1);
            if (w.find(one.at(0)) != std::string::npos && w.find(one.at(1)) != std::string::npos) {
                TO_NUMBERS[w] = 0;
                FROM_NUMBERS[0] = w;
                continue;
            }
            TO_NUMBERS[w] = 6;
            FROM_NUMBERS[6] = w;
        }
    }

    for (auto const& w : words) {
        if (w.size() == 5) {
            auto x = merge(w, FROM_NUMBERS.at(1));
            if (x.size() == 5) {
                TO_NUMBERS[w] = 3;
                FROM_NUMBERS[3] = w;
                continue;
            }
            x = merge(w, FROM_NUMBERS.at(4));
            if (x.size() == 7) {
                TO_NUMBERS[w] = 2;
                FROM_NUMBERS[2] = w;
                continue;
            }
            TO_NUMBERS[w] = 5;
            FROM_NUMBERS[5] = w;
        }
    }

    uint64_t result = 0;
    int mul = 1;
    auto it = words.rbegin();
    for (int i = 0; i < 4; ++i) {
        std::string const& w = *it;
        result += mul * TO_NUMBERS.at(w);
        mul *= 10;
        ++it;
    }
    return result;
}


int main()
{
    std::string line;

    WordList wlist;

    while(std::getline(std::cin, line)) {
        assert(!line.empty());

        Words words;
        boost::algorithm::split(words, line, boost::algorithm::is_any_of(" "));

        // 5th from the end is '|'
        assert(words.at(words.size() - 5) == "|");
        words.erase(std::next(words.begin(), words.size() - 5));

        for (auto& w : words) {
            std::sort(w.begin(), w.end());
        }

        wlist.push_back(std::move(words));
    }


    std::map<size_t, unsigned> wc;
    for (auto const& words : wlist) {
        auto it = words.rbegin();
        for (int i = 0; i < 4; ++i) {
            wc.insert({it->size(), 0}).first->second += 1;
            ++it;
        }
    }

    std::cout << "1: " << (wc[2] + wc[4] + wc[3] + wc[7]) << "\n";

    uint64_t sum = 0;
    for (auto const& words : wlist) {
        sum += decode(words);
    }

    std::cout << "2: " << sum << "\n";

    return 0;
}
