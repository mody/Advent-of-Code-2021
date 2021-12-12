#include <cassert>
#include <iostream>
#include <map>
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
    bool visited = false;
    bool capital = false;
    std::vector<std::string> children;
};

struct Tree
{
    void part1() {
        paths = 0;
        dive("start");
        std::cout << "1: " << paths << "\n";
    }

    void dive(std::string const& parent) {
        // std::cout << "dive to " << parent << "\n";
        if (parent == "end") {
            ++paths;
            // std::cout << "HIT end, " << paths << "times\n\n";
            return;
        }

        Node& top = nodes.at(parent);
        if (top.visited && !top.capital) {
            // can't use lower-case again
            // std::cout << "- can't use lower-case again\n";
            return;
        }
        top.visited = true;
        for (auto const& child : top.children) {
            dive(child);
        }

        // back-tracking, un-visit
        // std::cout << "- back-tracking, un-visit " << parent << "\n";
        top.visited = false;
    }

    std::map<std::string, Node> nodes;
    int paths = 0;
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

    return 0;
}
