#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>

struct Mapa {
    void append(std::string line) {
        max_x = line.size();
        max_y += 1;
        data.append(std::move(line));
    }

    size_t step() {
        for (auto& c : data) {
            ++c;
        }

        size_t flashes = 0;
        for(;;) {
            size_t i = 0;
            for (; i < data.size(); ++i) {
                if (data.at(i) > '9') {
                    break;
                }
            }
            if (i == data.size()) {
                break;
            }
            assert(data.at(i) > '9');
            data.at(i) = '0';
            ++flashes;

            auto const& [x, y] = index_to_xy(i);

            if (has(x - 1, y) && get(x - 1, y) != '0') { ++data.at(xy_to_index(x - 1, y)); }
            if (has(x + 1, y) && get(x + 1, y) != '0') { ++data.at(xy_to_index(x + 1, y)); }

            if (has(x, y - 1) && get(x, y - 1) != '0') { ++data.at(xy_to_index(x, y - 1)); }
            if (has(x, y + 1) && get(x, y + 1) != '0') { ++data.at(xy_to_index(x, y + 1)); }

            if (has(x - 1, y - 1) && get(x - 1, y - 1) != '0') { ++data.at(xy_to_index(x - 1, y - 1)); }
            if (has(x + 1, y + 1) && get(x + 1, y + 1) != '0') { ++data.at(xy_to_index(x + 1, y + 1)); }

            if (has(x + 1, y - 1) && get(x + 1, y - 1) != '0') { ++data.at(xy_to_index(x + 1, y - 1)); }
            if (has(x - 1, y + 1) && get(x - 1, y + 1) != '0') { ++data.at(xy_to_index(x - 1, y + 1)); }
        }
        return flashes;
    }

    void dump() const {
        for (size_t y = 0; y < max_y; ++y) {
            std::cout << data.substr(y * max_x, max_x) << "\n";
        }
    }

    size_t size() const {
        return data.size();
    }

protected:
    bool has(size_t x, size_t y) const
    {
        return (x >= 0 && y >= 0 && x < max_x && y < max_y);
    }

    char get(size_t x, size_t y) const
    {
        assert(has(x, y));
        return data.at(y * max_x + x);
    }

    size_t xy_to_index(size_t x, size_t y) const
    {
        return (y * max_x + x);
    }

    std::pair<size_t, size_t> index_to_xy(size_t index) const
    {
        size_t x = index % max_x;
        size_t y = index / max_y;
        return {x, y};
    }

    size_t max_x = 0, max_y = 0;
    std::string data;
};


int main()
{
    Mapa mapa;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        mapa.append(line);
    }

    size_t result1 = 0;
    size_t result2 = 0;
    for (size_t i = 0; i < 1000; ++i) {
        const size_t num = mapa.step();
        if (i < 100) {
            result1 += num;
        }
        if (result2 == 0 && num == mapa.size()) {
            result2 = i + 1;
            break;
        }
    }
    std::cout << "1: " << result1 << "\n";
    std::cout << "2: " << result2 << "\n";

    return 0;
}
