#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <set>
#include <stdexcept>
#include <string>

namespace {

int to_int(char c) {
    assert(c >= '0');
    assert(c <= '9');
    return c - '0';
}

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

    int64_t basin(int64_t x, int64_t y) const {
       std::set<size_t> visited;
       return basin_work(x, y, visited);
    }

protected:
    size_t xy_to_data(int64_t x, int64_t y) const
    {
        return (y * max_x + x);
    }

    int64_t basin_work(int64_t x, int64_t y, std::set<size_t>& visited) const {
        assert(has(x, y));
        if (!visited.insert(xy_to_data(x, y)).second) {
            return 0;
        }
        auto const& me = get(x, y);
        int64_t result = 1;
        if (has(x - 1, y) && get(x - 1, y) < '9' && me < get(x - 1, y)) { result += basin_work(x - 1, y, visited); }
        if (has(x + 1, y) && get(x + 1, y) < '9' && me < get(x + 1, y)) { result += basin_work(x + 1, y, visited); }
        if (has(x, y - 1) && get(x, y - 1) < '9' && me < get(x, y - 1)) { result += basin_work(x, y - 1, visited); }
        if (has(x, y + 1) && get(x, y + 1) < '9' && me < get(x, y + 1)) { result += basin_work(x, y + 1, visited); }
        return result;
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

    std::deque<std::pair<int64_t, int64_t>> lowest_list;

    int result1 = 0;
    for (int64_t y = 0; y < mapa.max_y; ++y) {
        for (int64_t x = 0; x < mapa.max_x; ++x) {
            if (mapa.is_lowest(x, y)) {
                result1 += to_int(mapa.get(x, y)) + 1;
                lowest_list.push_back({x, y});
            }
        }
    }
    std::cout << "1: " << result1 << "\n";

    std::deque<int64_t> basin_size;
    for (auto const& [x, y] : lowest_list) {
        basin_size.push_back(mapa.basin(x, y));
    }
    std::sort(basin_size.begin(), basin_size.end());

    int64_t result2 = 1;
    auto it = basin_size.rbegin();
    result2 = result2 * *it++;
    result2 = result2 * *it++;
    result2 = result2 * *it;

    std::cout << "2: " << result2 << "\n";

    return 0;
}
