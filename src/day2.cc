#include <cassert>
#include <iostream>
#include <string>

int main()
{
    int64_t depth = 0, pos = 0;

    std::string line;
    while(std::getline(std::cin, line)) {
        assert(!line.empty());
        if (line.at(0) == 'f') {  // forward
            pos += std::stol(line.substr(8));
        } else if (line.at(0) == 'd') {  // down
            depth += std::stol(line.substr(5));
        } else if (line.at(0) == 'u') {  // up
            depth -= std::stol(line.substr(2));
        }
    }

    std::cout << "depth: " << depth << ", pos: " << pos << "\n1: " << depth*pos << "\n";

    return 0;
}
