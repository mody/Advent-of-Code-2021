#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_selectors.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/pending/property.hpp>
#include <cassert>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>


struct Mapa
{
    using CoordType = int;

    void append(std::string const& line)
    {
        max_x = line.size();
        max_y += 1;
        for (auto c : line) {
            data.push_back(to_int(c));
        }
    }

    void dump() const
    {
        std::string line;
        line.reserve(max_x);
        for (CoordType y = 0; y < max_y; ++y) {
            for (CoordType x = 0; x < max_x; ++x) {
                line += to_char(get(x, y));
            }
            std::cout << line << "\n";
            line.clear();
        }
    }

    size_t size() const
    {
        return data.size();
    }

    int part1()
    {
        return calculate_distance();
    }

    int part2()
    {
        multiply();
        return calculate_distance();
    }

protected:
    void multiply()
    {
        std::vector<int> d2, line;
        for (CoordType y = 0; y < max_y; ++y) {
            for (CoordType x = 0; x < max_x; ++x) {
                int c = get(x, y);
                line.push_back(c);
                d2.push_back(c);
            }

            for (int i = 0; i < 4; ++i) {
                for (int& c : line) {
                    if (++c > 9) {
                        c = 1;
                    }
                    d2.push_back(c);
                }
            }

            line.clear();
        }

        std::swap(data, d2);
        max_x = max_x * 5;

        CoordType y = 0;
        for (int i = 0; i < 4 * max_y; ++i) {
            for (CoordType x = 0; x < max_x; ++x) {
                auto c = data.at(xy_to_index(x, y));
                if (++c > 9) {
                    c = 1;
                }
                data.push_back(c);
            }
            ++y;
        }
        max_y = max_y * 5;
    }

    int calculate_distance() const
    {
        using Graph = boost::adjacency_list<
            boost::vecS, boost::vecS, boost::bidirectionalS, boost::property<boost::vertex_distance_t, int>,
            boost::property<boost::edge_weight_t, int>>;

        Graph g;

        for (size_t idx = 0; idx < data.size(); ++idx) {
            (void)add_vertex(g);
        }

        auto ew = boost::get(boost::edge_weight, g);

        for (size_t idx = 0; idx < data.size(); ++idx) {
            auto const& [x, y] = index_to_xy(idx);
            auto from = boost::vertex(idx, g);

            auto add_edge = [&](Mapa::CoordType x, Mapa::CoordType y) {
                auto const& [e, _] = boost::add_edge(from, xy_to_index(x, y), g);
                ew[e] = get(x, y);
            };

            if (has(x - 1, y)) { add_edge(x - 1, y); }
            if (has(x + 1, y)) { add_edge(x + 1, y); }

            if (has(x, y - 1)) { add_edge(x, y - 1); }
            if (has(x, y + 1)) { add_edge(x, y + 1); }
        }

        boost::dijkstra_shortest_paths(
            g, boost::vertex(0, g), boost::distance_map(boost::get(boost::vertex_distance, g)));

        auto dist = boost::get(boost::vertex_distance, g);
        return dist[boost::vertex(data.size() - 1, g)];
    }

    bool has(CoordType x, CoordType y) const
    {
        return (x >= 0 && y >= 0 && x < max_x && y < max_y);
    }

    char get(CoordType x, CoordType y) const
    {
        assert(has(x, y));
        return data.at(y * max_x + x);
    }

    CoordType xy_to_index(CoordType x, CoordType y) const
    {
        return (y * max_x + x);
    }

    std::pair<CoordType, CoordType> index_to_xy(CoordType index) const
    {
        CoordType x = index % max_x;
        CoordType y = index / max_x;
        return {x, y};
    }

    int to_int(char c) const
    {
        assert(c >= '0');
        assert(c <= '9');
        return c - '0';
    }

    char to_char(int i) const
    {
        assert(i >= 0);
        assert(i <= 9);
        return i + '0';
    }

    CoordType max_x = 0, max_y = 0;
    std::vector<int> data;
};


int main()
{
    Mapa mapa;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        mapa.append(line);
    }

    auto r1 = mapa.part1();
    std::cout << "1: " << r1 << "\n";

    auto r2 = mapa.part2();
    std::cout << "2: " << r2 << "\n";

    return 0;
}
