#include <algorithm>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <vector>

auto main() -> int
{
    // Request to user if he wants to delete 50% of the files
    std::cout << "Files have 50% chance to be deleted. Do you want to continue? (Y/n): ";
    std::string answer = "";
    std::cin >> answer;

    if (answer != "Y") {
        return 0;
    }

    // List all files in the current directory
    std::vector<std::string> list;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(".")) {
      list.emplace_back(entry.path().string());
    }

    // Shuffle the list with a random generator
    std::random_device rd;
    std::mt19937_64 rng(rd());

    std::shuffle(list.begin(), list.end(), rng);

    // Remove the first half of the list
    list.erase(list.begin() + (list.size() / 2), list.end());

    // Delete all files in the list
    for (auto& file : list) {
        std::filesystem::remove(file);
    }

    std::cout << "Thanos snapped his fingers." << std::endl;

    return 0;
}
