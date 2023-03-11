#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

auto main() -> int
{   
    constexpr std::size_t max_depth = 5;

    namespace fs = std::filesystem;

    if (fs::exists("recursive")) {
        fs::remove_all("recursive");
    }

    fs::current_path(fs::temp_directory_path());

    for (std::size_t i = 0; i < max_depth; i++) {
        fs::create_directories("recursive");
        fs::current_path("recursive");
    }

    std::ofstream file("test.txt");
    file << "Hello world!";
    file.close();

    fs::current_path(fs::temp_directory_path());
    std::system("tree recursive");
    fs::remove_all("recursive");
    return 0;
}
