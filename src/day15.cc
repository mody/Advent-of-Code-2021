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

    void append(std::string line)
    {
        max_x = line.size();
        max_y += 1;
        data.append(std::move(line));
    }

    void dump() const
    {
        for (CoordType y = 0; y < max_y; ++y) {
            std::cout << data.substr(y * max_x, max_x) << "\n";
        }
    }

    size_t size() const
    {
        return data.size();
    }

    std::string const& get_data() const
    {
        return data;
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
        CoordType y = index / max_y;
        return {x, y};
    }

protected:
    CoordType max_x = 0, max_y = 0;
    std::string data;
};

template<typename T> class TD;

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

    // mapa.dump();

    using vertex_descriptor =
        boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::bidirectionalS>::vertex_descriptor;

    using Graph = boost::adjacency_list<
        boost::vecS, boost::vecS, boost::bidirectionalS,
        boost::property<boost::vertex_distance_t, int, boost::property<boost::vertex_predecessor_t, vertex_descriptor>>,
        boost::property<boost::edge_weight_t, int>>;

    Graph g;

    for (size_t idx = 0; idx < mapa.get_data().size(); ++idx) {
        (void)add_vertex(g);
    }

    auto ew = boost::get(boost::edge_weight, g);

    for (size_t idx = 0; idx < mapa.get_data().size(); ++idx) {
        auto const& [x, y] = mapa.index_to_xy(idx);
        auto from = boost::vertex(idx, g);

        auto add_edge = [&](Mapa::CoordType x, Mapa::CoordType y) {
            auto const& [e, _] = boost::add_edge(from, mapa.xy_to_index(x, y), g);
            ew[e] = mapa.get(x, y) - '0';
        };

        if (mapa.has(x - 1, y)) { add_edge(x - 1, y); }
        if (mapa.has(x + 1, y)) { add_edge(x + 1, y); }

        if (mapa.has(x, y - 1)) { add_edge(x, y - 1); }
        if (mapa.has(x, y + 1)) { add_edge(x, y + 1); }

        // if (mapa.has(x - 1, y - 1)) { add_edge(x - 1, y - 1); }
        // if (mapa.has(x + 1, y + 1)) { add_edge(x + 1, y + 1); }

        // if (mapa.has(x + 1, y - 1)) { add_edge(x + 1, y - 1); }
        // if (mapa.has(x - 1, y + 1)) { add_edge(x - 1, y + 1); }
    }

    boost::dijkstra_shortest_paths(
        g, boost::vertex(0, g),
        predecessor_map(boost::get(boost::vertex_predecessor, g)).distance_map(boost::get(boost::vertex_distance, g)));

    auto dist = boost::get(boost::vertex_distance, g);
    std::cout << "1: " << dist[boost::vertex(mapa.get_data().size()-1, g)] << "\n";

    return 0;
}
