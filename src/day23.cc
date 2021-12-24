#include <algorithm>
#include <boost/config.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/detail/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/properties.hpp>
#include <boost/pending/property.hpp>

#include <cassert>
#include <deque>
#include <ios>
#include <iostream>
#include <unordered_set>

struct Position
{
    unsigned x = 0, y = 0;
    char id = 0;
    bool done = false;

    bool operator==(Position const& o) const noexcept
    {
        return x == o.x && y == o.y && id == o.id && done == o.done;
    }

    bool operator<(Position const& o) const noexcept
    {
        return id < o.id ? true : (x < o.x ? true : (y < o.y ? true : done < o.done));
    }

    friend std::size_t hash_value(Position const& p) noexcept
    {
        size_t seed = 0;
        boost::hash_combine(seed, p.x);
        boost::hash_combine(seed, p.y);
        boost::hash_combine(seed, p.id);
        boost::hash_combine(seed, p.done);
        return seed;
    }
};

using Positions = std::vector<Position>;

struct State
{
    State() = default;

    explicit State(Positions _p)
        : players {std::move(_p)}
    { }

    State(State const&) = default;
    State(State&&) = default;

    State& operator=(State const&) = default;
    State& operator=(State&&) = default;

    bool operator==(State const& o) const noexcept
    {
        return boost::hash_value(players) == boost::hash_value(o.players);
    }

    bool occupied(Position const& p) const noexcept
    {
        auto it = std::find_if(
            players.cbegin(), players.cend(), [&p](Position const& o) { return p.x == o.x && p.y == o.y; });
        return it != players.cend();
    }

    Position const& at(Position const& p) const noexcept
    {
        auto it = std::find_if(
            players.cbegin(), players.cend(), [&p](Position const& o) { return p.x == o.x && p.y == o.y; });
        assert(it != players.cend());
        return *it;
    }

    void move_player(Position const& p, Position const& dst)
    {
        auto it = std::find_if(players.begin(), players.end(), [&p](Position const& o) {
            return p.x == o.x && p.y == o.y && p.id == o.id;
        });
        assert(it != players.end());
        assert(fuel_needed(p, dst));
        it->x = dst.x;
        it->y = dst.y;
        std::sort(players.begin(), players.end());
    }

    void set_player_home(Position const& p, bool val = true)
    {
        auto it = std::find_if(players.begin(), players.end(), [&p](Position const& o) {
            return p.x == o.x && p.y == o.y && p.id == o.id;
        });
        assert(it != players.end());
        it->done = val;
        std::sort(players.begin(), players.end());
    }

    unsigned fuel_needed(Position start, Position const& end) const noexcept
    {
        static const std::map<char, unsigned> FUEL_MODIFIER {{'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000}};

        assert(occupied(start));
        assert(!occupied(end));
        unsigned steps = 0;
        // move from the room
        for (; start.y > 0;) {
            --start.y;
            if (occupied(start)) {
                return 0;
            }
            ++steps;
        }
        // move along the hallway
        assert(start.x != end.x);
        int dir = (start.x < end.x ? 1 : -1);
        for (; start.x != end.x;) {
            start.x += dir;
            if (occupied(start)) {
                return 0;
            }
            ++steps;
        }
        // move to a room
        for (; start.y != end.y;) {
            ++start.y;
            if (occupied(start)) {
                return 0;
            }
            ++steps;
        }
        return steps * FUEL_MODIFIER.at(start.id);
    }

    Positions const& get_players() const noexcept
    {
        return players;
    }

    void fix_done(char id, unsigned x)
    {
        auto it = std::find(players.begin(), players.end(), Position {.x = x, .y = 2, .id = id, .done = false});
        if (it != players.end()) {
            it->done = true;
            it = std::find(players.begin(), players.end(), Position {.x = x, .y = 1, .id = id, .done = false});
            if (it != players.end()) {
                it->done = true;
            }
        }
        std::sort(players.begin(), players.end());
    }

    void add_player(Position p) noexcept
    {
        players.push_back(std::move(p));
        std::sort(players.begin(), players.end());
    }

protected:
    Positions players;
};

namespace std {

std::ostream& operator<<(std::ostream& os, Position const& p)
{
    os << "(" << p.id << ";" << p.x << ";" << p.y << ";" << p.done << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, State const& s)
{
    std::cout << "#############\n#";
    for (unsigned x = 1; x < 12; ++x) {
        if (s.occupied({.x = x, .y = 0})) {
            std::cout << s.at({.x = x, .y = 0}).id;
        } else {
            std::cout << " ";
        }
    }
    std::cout << "#\n###";
    for (unsigned y = 1; y < 3; ++y) {
        for (unsigned x = 3; x < 10; x += 2) {
            if (s.occupied({.x = x, .y = y})) {
                std::cout << s.at({.x = x, .y = y}).id;
            } else {
                std::cout << " ";
            }
            std::cout << "#";
        }
        std::cout << "##\n###";
    }
    std::cout << "##########\n";

    std::cout << std::boolalpha;
    for (auto const& p : s.get_players()) {
        std::cout << p << ",";
    }
    return os;
}

 template<>
 struct hash<State>
 {
     size_t operator()(State const& s) const noexcept
     {
        return boost::hash_range(s.get_players().begin(), s.get_players().end());
     }
 };


}  // namespace std


//
//   0123456789012
// 0 #...........#
// 1 ###B#C#B#D###
// 2   #A#D#C#A#

static const Positions HALLWAY_POSITIONS {{.x = 1, .y = 0}, {.x = 2, .y = 0},  {.x = 4, .y = 0}, {.x = 6, .y = 0},
                                          {.x = 8, .y = 0}, {.x = 10, .y = 0}, {.x = 11, .y = 0}};

static const State END_STATE {{
    {.x = 3, .y = 1, .id = 'A', .done = true},
    {.x = 5, .y = 1, .id = 'B', .done = true},
    {.x = 7, .y = 1, .id = 'C', .done = true},
    {.x = 9, .y = 1, .id = 'D', .done = true},
    {.x = 3, .y = 2, .id = 'A', .done = true},
    {.x = 5, .y = 2, .id = 'B', .done = true},
    {.x = 7, .y = 2, .id = 'C', .done = true},
    {.x = 9, .y = 2, .id = 'D', .done = true},
}};

static const std::map<char, unsigned> HOME_ROOM_X {{'A', 3}, {'B', 5}, {'C', 7}, {'D', 9}};


uint64_t part1(State start)
{
    using EdgeWeight = boost::property<boost::edge_weight_t, unsigned>;
    using VertexDistance = boost::property<boost::vertex_distance_t, unsigned>;
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, VertexDistance, EdgeWeight>;
    using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

    Graph g;

    std::unordered_map<State, Vertex> states;
    std::deque<State> states_queue;

    Vertex start_v = boost::add_vertex(VertexDistance {0}, g);
    states.insert({start, start_v});
    states_queue.push_back(start);

    while (!states_queue.empty()) {
        const State s = states_queue.front();
        states_queue.pop_front();

        if (s == END_STATE) {
            std::cout << "HIT END" << std::endl;
            // final state needs no expansion
            continue;
        }

        // std::cout << "State:\n" << s << "\n";

        const Vertex from = states.at(s);

        // expand the state to all possibilities
        for (auto const& p : s.get_players()) {
            if (p.done) {
                continue;
            }

            //   0123456789012
            // 0 #...........#
            // 1 ###.#.#.#.###
            // 2   #.#.#.#.#
            //     #########

            Position dst1{.x = HOME_ROOM_X.at(p.id), .y = 1};
            Position dst2{.x = HOME_ROOM_X.at(p.id), .y = 2};

            Positions possible_moves;
            if (!s.occupied(dst2)) {
                if (!s.occupied(dst1)) {
                    possible_moves.push_back(std::move(dst2));
                }
            } else {
                if (s.at(dst2).done) {
                    if (!s.occupied(dst1)) {
                        possible_moves.push_back(std::move(dst1));
                    }
                }
            }
            if (p.y > 0) {
                // if not in hallway, add hallway
                possible_moves.insert(possible_moves.end(), HALLWAY_POSITIONS.begin(), HALLWAY_POSITIONS.end());
            }

            for (auto const& dst : possible_moves) {
                if (s.occupied(dst)) {
                    continue;
                }
                const unsigned fuel = s.fuel_needed(p, dst);
                if (!fuel) {
                    continue;
                }
                // add new state
                State s2 = s;
                s2.set_player_home(p, dst.y > 0);
                s2.move_player(p, dst);
                auto it = states.find(s2);
                if (it == states.end()) {
                    Vertex to = boost::add_vertex(VertexDistance {0}, g);
                    auto const& [_, in] = boost::add_edge(from, to, EdgeWeight {fuel}, g);
                    assert(in);
                    states.insert({s2, std::move(to)});
                    // std::cout << "State:\n" << s2 << "\nFuel: " << fuel << " from " << p << "\n\n";
                    states_queue.push_back(std::move(s2));
                } else {
                    boost::add_edge(from, it->second, EdgeWeight {fuel}, g);
                }
            }
        }
    }

    std::cout << "States: " << states.size() << std::endl;

    auto dist_map = boost::get(boost::vertex_distance, g);
    Vertex dst = states.at(END_STATE);

    std::vector<Vertex> pred(boost::num_vertices(g));
    boost::dijkstra_shortest_paths(g, start_v, boost::distance_map(dist_map).predecessor_map(&pred[0]));

    {
        std::map<Vertex, State> vx_to_state;
        for (auto const& [s, vx] : states) {
            auto p = vx_to_state.insert({vx, s});
            assert(p.second);
        }
        std::vector<Vertex> path;
        for (Vertex i = dst; i != start_v;i = pred[i]) {
            path.push_back(i);
        }
        path.push_back(start_v);
        std::reverse(path.begin(), path.end());

        for (Vertex vx : path) {
            std::cout << vx_to_state.at(vx) << "\n\n";
        }
    }

    return dist_map[dst];
}

int main()
{
    State start;

    std::string line;

    // skip hallway
    std::getline(std::cin, line);
    std::getline(std::cin, line);

    Position p {.y = 1};
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        for (p.x = 0; p.x < line.size(); ++p.x) {
            auto const& c = line.at(p.x);
            if (c == '#' || c == ' ') {
                continue;
            }
            assert(c >= 'A');
            assert(c <= 'D');
            p.id = c;
            start.add_player(p);
        }
        ++p.y;
    }

    // fix `done` in `start` position
    start.fix_done('A', 3);
    start.fix_done('B', 5);
    start.fix_done('C', 7);
    start.fix_done('D', 9);

    auto r1 = part1(start);
    std::cout << "1: " << r1 << "\n";

    // 19052 - too high
    // 19046 - CORRECT

    return 0;
}
