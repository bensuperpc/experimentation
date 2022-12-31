#include <algorithm>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "experimentation/filesystem.hpp"

auto main() -> int
{
    // Request to user if he wants to delete 50% of the files
    std::cout << "Files have 50% chance to be deleted. Do you want to continue? (y/n): ";
    std::string answer;
    std::cin >> answer;

    if (answer != "y") {
        return 0;
    }

    // List all files in the current directory
    std::vector<std::string> list;
    benlib::filesystem::experimentation::list_all_files(list, ".");
    /*
    for (const auto& entry : std::filesystem::recursive_directory_iterator(".")) {
      list.emplace_back(entry.path().string());
    }
    */

    // Shuffle the list with a random generator
    std::random_device rd;
    const std::uint64_t seed = (static_cast<std::uint64_t>(rd()) << 32) | rd();
    std::mt19937_64 rng(seed);

    std::shuffle(list.begin(), list.end(), rng);

    // Delete the first half of the list
    for (decltype(list.size()) i = 0; i < list.size() / 2; ++i) {
        std::filesystem::remove(list[i]);
    }

    std::cout << "Thanos snapped his fingers." << std::endl;

    return 0;
}
