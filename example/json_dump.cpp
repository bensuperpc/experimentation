#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

auto main() -> int
{
    std::ifstream file("test.json");
    nlohmann::json j = nlohmann::json::parse(file);
    const auto s = j.dump();
    std::cout << "JSON string: " << s << std::endl;
    std::cout << "JSON size  : " << s.size() << std::endl;
    return 0;
}
