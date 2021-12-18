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
        // std::cout << "Add: " << input << std::endl;
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
        // std::cout << "Explode..." << std::endl;
        Node* hit = nullptr;
        iterate(root, 1, [&](Node* n, int level) -> bool {
            // std::cout << "OP: " << std::hex << n << ", level: " << level << std::endl;
            if (level > 4) {
                hit = n;
                return true;
            }
            return false;
        });

        if (!hit) {
            // std::cout << "nothing to explode" << std::endl;
            return false;
        }
        assert(hit->left == nullptr);
        assert(hit->right == nullptr);
        // std::cout << std::dec << "hit [" << hit->lval << "," << hit->rval << "]" << std::endl;

        Node* l = hit;
        for(; l->parent && l == l->parent->left; l = l->parent) {}
        if (l->parent != nullptr) {
            // handle left side
            l = l->parent;
            if (l->left) {
                l = l->left;
                for (; l->right; l = l->right) {
                }
                // std::cout << "left target value in " << std::hex << l << ": " << std::dec << l->rval << std::endl;
                l->rval += hit->lval;
            } else {
                // std::cout << "left target value in " << std::hex << l << ": " << std::dec << l->lval << std::endl;
                l->lval += hit->lval;
            }
        } else {
            // std::cout << "NO left target value found" << std::endl;
        }

        Node* r = hit;
        for(; r->parent && r == r->parent->right; r = r->parent) {}
        if (r->parent != nullptr) {
            // handle left side
            r = r->parent;
            if (r->right) {
                r = r->right;
                for(; r->left; r = r->left) {}
                // std::cout << "right target value in " << std::hex << r << ": " << std::dec << r->lval << std::endl;
                r->lval += hit->rval;
            } else {
                // std::cout << "right target value in " << std::hex << r << ": " << std::dec << r->rval << std::endl;
                r->rval += hit->rval;
            }
        } else {
            // std::cout << "NO right target value found" << std::endl;
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
        // std::cout << "Split..." << std::endl;
        std::stringstream ss;
        dumper(ss, root);
        const std::string dump_str = ss.str();
        // std::cout << "dump_str: " << dump_str << std::endl;

        Node* hit = nullptr;
        iterate_for_split(root, [&](Node* n, int val) -> bool {
            // std::cout << "for split - see: " << val << std::endl;
            if (val >= 10) {
                hit = n;
                return true;
            }
            return false;
        });

        if (!hit) {
            // std::cout << "nothing to split" << std::endl;
            return false;
        }
        // std::cout << std::dec << "hit [" << hit->lval << "," << hit->rval << "]" << std::endl;

        {
            std::smatch m;
            bool found = std::regex_search(dump_str, m, NUMBER);
            assert(found);
            auto num = std::stoi(m.str(1));
            if (hit->lval > 9) {
                // std::cout << "num: " << num << " != " << hit->lval << std::endl;
                assert(num == hit->lval);
            } else {
                // std::cout << "num: " << num << " != " << hit->rval << std::endl;
                assert(num == hit->rval);
            }
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
        // std::cout << std::hex << p << " - input: " << input << std::endl;

        std::smatch m;
        if (std::regex_search(input, m, ONE)) {
            p->lval = std::stoi(m.str(1));
            p->rval = std::stoi(m.str(2));
            input = input.substr(m.str(0).size());
        } else {
            // std::cout << std::hex << p << " - l-input: " << input << std::endl;
            if (std::regex_search(input, m, L_SIDE)) {
                p->lval = std::stoi(m.str(1));
                input = input.substr(m.str(0).size());
            } else {
                input = input.substr(1);  // skip '['
                p->left = parse(p.get(), input);
            }

            // std::cout << std::hex << p << " - r-input: " << input << std::endl;
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
        // std::cout << std::hex << p << " - output: " << input << std::endl;

        return p.get();
    }

    Node* root = nullptr;
    std::vector<NodePtr> nodes;
};

/*
[[[[4,3],4],4],[7,[[8,4],9]]] + [1,1]:

after addition: [[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]
after explode:  [[[[0,7],4],[7,[[8,4],9]]],[1,1]]
after explode:  [[[[0,7],4],[15,[0,13]]],[1,1]]
after split:    [[[[0,7],4],[[7,8],[0,13]]],[1,1]]
after split:    [[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]
after explode:  [[[[0,7],4],[[7,8],[6,0]]],[8,1]]
*/

int main()
{
    Tree tree;

    std::string line;
    std::getline(std::cin, line);
    assert(!line.empty());
    tree.add(line);

    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        tree.add(line);
        // tree.dump();

        for (;;) {
            if (tree.explode()) {
                // tree.dump();
                continue;
            }
            if (tree.split()) {
                continue;
            }
            break;
        }
        // tree.dump();
    }
    tree.dump();
    std::cout << "1: " << tree.magnitude() << "\n";


    return 0;
}
