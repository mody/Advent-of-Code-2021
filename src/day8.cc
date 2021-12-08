#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <map>
#include <string>

int main()
{
    std::string line;

    std::map<size_t, unsigned> wc;

    while(std::getline(std::cin, line)) {
        assert(!line.empty());

        auto pipe = line.find('|');
        assert(pipe != std::string::npos);

        ++pipe;
        assert(pipe < line.size());

        std::vector<std::string> nums;
        boost::algorithm::split(nums, line.substr(pipe), boost::algorithm::is_any_of(" "));

        for (auto const& w : nums) {
            wc.insert({w.size(), 0}).first->second += 1;
        }
    }

    std::cout << "1: " << (wc.at(2)+wc.at(4)+wc.at(3)+wc.at(7)) << "\n";

    return 0;
}
