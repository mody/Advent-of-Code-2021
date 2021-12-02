#include <cassert>
#include <iostream>
#include <string>

int main()
{
    int64_t depth = 0, pos = 0;
    int64_t depth2 = 0, pos2 = 0, aim = 0;

    std::string line;
    while(std::getline(std::cin, line)) {
        assert(!line.empty());
        if (line.at(0) == 'f') {  // forward
            pos += std::stol(line.substr(8));
            pos2 += std::stol(line.substr(8));
            depth2 += aim * std::stol(line.substr(8));
        } else if (line.at(0) == 'd') {  // down
            depth += std::stol(line.substr(5));
            aim += std::stol(line.substr(5));
        } else if (line.at(0) == 'u') {  // up
            depth -= std::stol(line.substr(2));
            aim -= std::stol(line.substr(2));
        }
    }

    std::cout << "depth: " << depth << ", pos: " << pos << "\n1: " << depth*pos << "\n";
    std::cout << "depth2: " << depth2 << ", pos2: " << pos2 << "\n1: " << depth2*pos2 << "\n";

    return 0;
}
