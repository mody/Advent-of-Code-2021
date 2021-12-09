#include <cassert>
#include <deque>
#include <iostream>
#include <string>

namespace {

int to_int(char c) {
    assert(c >= '0');
    assert(c <= '9');
    return c - '0';
}

// char to_char(int i) {
//     assert(i >= 0);
//     assert(i <= 9);
//     return i + '0';
// }

}  // namespace

struct Mapa {
    int64_t max_x = 0, max_y = 0;
    std::string data;

    bool has(int64_t x, int64_t y) const {
        return (x >= 0 && y >= 0 && x < max_x && y < max_y);
    }

    char get(int64_t x, int64_t y) const {
        assert(has(x, y));
        return data.at(y * max_x + x);
    }

    bool is_lowest(int64_t x, int64_t y) const {
        assert(has(x, y));
        auto const& me = get(x, y);
        if (has(x - 1, y) && me >= get(x - 1, y)) { return false; }
        if (has(x + 1, y) && me >= get(x + 1, y)) { return false; }
        if (has(x, y - 1) && me >= get(x, y - 1)) { return false; }
        if (has(x, y + 1) && me >= get(x, y + 1)) { return false; }
        return true;
    }
};


int main()
{
    Mapa mapa;

    std::string line;
    while(std::getline(std::cin, line)) {
        mapa.max_x = line.size();
        mapa.max_y += 1;
        mapa.data.append(line);
    }
    // for (int64_t y = 0; y < mapa.max_y; ++y) {
    //     for (int64_t x = 0; x < mapa.max_x; ++x) {
    //         std::cout << mapa.get(x,y);
    //     }
    //     std::cout << "\n";
    // }

    int result1 = 0;
    for (int64_t y = 0; y < mapa.max_y; ++y) {
        for (int64_t x = 0; x < mapa.max_x; ++x) {
            if (mapa.is_lowest(x, y)) {
                // std::cout << "found lowes at " << x << ";" << y << " value: " << mapa.get(x,y) << "\n";
                result1 += to_int(mapa.get(x, y)) + 1;
            }
        }
    }

    std::cout << "1: " << result1 << "\n";

    return 0;
}
