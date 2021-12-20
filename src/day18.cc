#include <cassert>
#include <iostream>
#include <iterator>
#include <memory>
#include <regex>
#include <sstream>
#include <string>

static std::regex ONE{"^\\[(\\d+),(\\d+)\\]"};
static std::regex L_SIDE{"^\\[(\\d+),"};
static std::regex R_SIDE{"^,(\\d+)\\]"};
static std::regex NUMBER{"(\\d\\d+)"};

struct Node {
    Node *parent = nullptr, *left = nullptr, *right = nullptr;

    unsigned lval = 0, rval = 0;
};
using NodePtr = std::shared_ptr<Node>;

struct Tree {
    using NodeOp = std::function<bool(Node*, int)>;

    void add(std::string input) {
        if (root == nullptr) {
            root = parse(nullptr, input);
            return;
        }

        nodes.push_back(std::make_shared<Node>());
        auto p = nodes.back();

        p->left = root;
        root->parent = p.get();
        p->right = parse(p.get(), input);
        root = p.get();
    }

    bool explode()
    {
        Node* hit = nullptr;
        iterate(root, 1, [&](Node* n, int level) -> bool {
            if (level > 4) {
                hit = n;
                return true;
            }
            return false;
        });

        if (!hit) {
            return false;
        }
        assert(hit->left == nullptr);
        assert(hit->right == nullptr);

        Node* l = hit;
        for(; l->parent && l == l->parent->left; l = l->parent) {}
        if (l->parent != nullptr) {
            l = l->parent;
            if (l->left) {
                l = l->left;
                for (; l->right; l = l->right) {
                }
                l->rval += hit->lval;
            } else {
                l->lval += hit->lval;
            }
        }

        Node* r = hit;
        for(; r->parent && r == r->parent->right; r = r->parent) {}
        if (r->parent != nullptr) {
            r = r->parent;
            if (r->right) {
                r = r->right;
                for(; r->left; r = r->left) {}
                r->lval += hit->rval;
            } else {
                r->rval += hit->rval;
            }
        }

        if (hit == hit->parent->left) {
            hit->parent->lval = 0;
            hit->parent->left = nullptr;
        } else {
            assert(hit == hit->parent->right);
            hit->parent->rval = 0;
            hit->parent->right = nullptr;
        }

        return true;
    }

    bool split()
    {
        Node* hit = nullptr;
        iterate_for_split(root, [&](Node* n, int val) -> bool {
            if (val >= 10) {
                hit = n;
                return true;
            }
            return false;
        });

        if (!hit) {
            return false;
        }

        nodes.push_back(std::make_shared<Node>());
        auto p = nodes.back();
        p->parent = hit;
        if (hit->lval >= 10) {
            p->lval = hit->lval / 2;
            p->rval = hit->lval - p->lval;
            hit->lval = 0;
            hit->left = p.get();
        } else {
            assert(hit->rval >= 10);
            p->lval = hit->rval / 2;
            p->rval = hit->rval - p->lval;
            hit->rval = 0;
            hit->right = p.get();
        }

        return true;
    }

    void dump() const {
        dumper(std::cout, root);
        std::cout << "\n";
    }

    uint64_t magnitude() const
    {
        return iterate_for_magnitude(root);
    }

protected:
    template<typename OStream>
    void dumper(OStream& os, Node* n) const
    {
        os << "[";
        if (n->left) {
            dumper(os, n->left);
        } else {
            os << std::dec << n->lval;
        }
        os << ",";

        if (n->right) {
            dumper(os, n->right);
        } else {
            os << std::dec << n->rval;
        }
        os << "]";
    }

    bool iterate(Node* n, unsigned level, NodeOp const& op) const
    {
        if (op(n, level)) {
            return false;
        };
        if (n->left) {
            if (!iterate(n->left, level + 1, op)) {
                return false;
            }
        }
        if (n->right) {
            return iterate(n->right, level + 1, op);
        }
        return true;
    }

    bool iterate_for_split(Node* n, NodeOp const& op) const
    {
        if (n->left) {
            if (!iterate_for_split(n->left, op)) {
                return false;
            }
        } else if (op(n, n->lval)) {
            return false;
        }
        if (n->right) {
            if (!iterate_for_split(n->right, op)) {
                return false;
            }
        } else if (op(n, n->rval)) {
            return false;
        }
        return true;
    }

    uint64_t iterate_for_magnitude(Node* n) const
    {
        uint64_t result = 0;
        if (n->left) {
            result += 3 * iterate_for_magnitude(n->left);
        } else {
            result += 3 * n->lval;
        }
        if (n->right) {
            result += 2 * iterate_for_magnitude(n->right);
        } else {
            result += 2 * n->rval;
        }
        return result;
    }

    Node* parse(Node* parent, std::string& input) {
        nodes.push_back(std::make_shared<Node>());
        auto p = nodes.back();
        p->parent = parent;

        std::smatch m;
        if (std::regex_search(input, m, ONE)) {
            p->lval = std::stoi(m.str(1));
            p->rval = std::stoi(m.str(2));
            input = input.substr(m.str(0).size());
        } else {
            if (std::regex_search(input, m, L_SIDE)) {
                p->lval = std::stoi(m.str(1));
                input = input.substr(m.str(0).size());
            } else {
                input = input.substr(1);  // skip '['
                p->left = parse(p.get(), input);
            }

            if (input.front() == '[') {
                p->right = parse(p.get(), input);
                input = input.substr(1);  // skip ']'
            } else if (std::regex_search(input, m, R_SIDE)) {
                p->rval = std::stoi(m.str(1));
                input = input.substr(m.str(0).size());
            } else {
                input = input.substr(1);  // skip ','
                p->right = parse(p.get(), input);
                input = input.substr(1);  // skip ']'
            }
        }

        return p.get();
    }

    Node* root = nullptr;
    std::vector<NodePtr> nodes;
};

void process_tree(Tree& tree) {
    for (;;) {
        if (tree.explode()) {
            continue;
        }
        if (tree.split()) {
            continue;
        }
        break;
    }
}

int main()
{
    Tree tree;

    std::vector<std::string> lines;

    {
        std::string line;
        while (std::getline(std::cin, line)) {
            if (line.empty()) {
                break;
            }
            lines.push_back(std::move(line));
        }
    }

    for (auto const& l : lines) {
        tree.add(l);
        process_tree(tree);
    }
    std::cout << "1: " << tree.magnitude() << "\n";

    uint64_t result2 = 0;
    for (auto i1 = lines.begin(); i1 != lines.end(); ++i1) {
        for (auto i2 = lines.begin(); i2 != lines.end(); ++i2) {
            if (i1 == i2) {
                continue;
            }
            Tree t2;
            t2.add(*i1);
            process_tree(t2);
            t2.add(*i2);
            process_tree(t2);
            result2 = std::max(result2, t2.magnitude());
        }
    }

    std::cout << "2: " << result2 << "\n";

    return 0;
}
