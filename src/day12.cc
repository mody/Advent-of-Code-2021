#include <cassert>
#include <deque>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

struct Node
{
    Node(std::string _name) : name{std::move(_name)}
    {
        capital = name.front() >= 'A' && name.front() <= 'Z';
    }

    void add_child(std::string name) {
        children.push_back(std::move(name));
    }

    std::string name;
    int visited = 0;
    int max_visits = 1;
    bool capital = false;
    std::vector<std::string> children;
};

struct Tree
{
    void part1() {
        paths.clear();
        dive("start");
        std::cout << "1: " << paths.size() << "\n";
    }

    void part2() {
        paths.clear();

        for (auto& n : nodes) {
            if (n.second.capital || n.first == "start" || n.first == "end") {
                continue;
            }
            n.second.max_visits = 2;
            dive("start");
            n.second.max_visits = 1;
        }
        std::cout << "2: " << paths.size() << "\n";
    }

    void dive(std::string const& parent) {
        if (parent == "end") {
            std::string path_string;
            for (auto const& p : path) {
                path_string.append(p).append(",");
            }
            path_string.append("end");
            paths.insert(std::move(path_string));
            return;
        }

        Node& top = nodes.at(parent);
        if (!top.capital && top.visited == top.max_visits) {
            // can't use lower-case again
            return;
        }
        path.push_back(parent);
        ++top.visited;
        for (auto const& child : top.children) {
            dive(child);
        }

        // back-tracking, un-visit
        --top.visited;
        path.pop_back();
    }

    std::map<std::string, Node> nodes;
    int paths_count = 0;
    std::deque<std::string> path;
    std::set<std::string> paths;
};

int main()
{
    Tree tree;

    std::string line;
    while (std::getline(std::cin, line)) {
        auto split = line.find('-');
        assert(split != std::string::npos);
        std::string from = line.substr(0, split);
        std::string to = line.substr(split + 1);
        {
            auto p = tree.nodes.insert({from, {from}});
            p.first->second.add_child(to);
        }
        {
            auto p = tree.nodes.insert({to, {to}});
            p.first->second.add_child(from);
        }
    }

    tree.part1();
    tree.part2();

    return 0;
}
