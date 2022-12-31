#include <iostream>
#include <string>
#include <vector>

#include "experimentation/filesystem.hpp"

auto main() -> int
{
    std::vector<std::string> list;
    benlib::filesystem::experimentation::list_all_files(list, ".");
    for (const auto& file : list) {
        std::cout << file << std::endl;
    }
    return 0;
}
