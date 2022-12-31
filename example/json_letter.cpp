#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

auto main() -> int
{
    std::ifstream file("test.json");

    if (!file.is_open()) {
        std::cout << "File not found" << std::endl;
        return 1;
    }

    nlohmann::json j = nlohmann::json::parse(file);
    // std::string email = j.at(0).at("email");

    for (const auto& item : j.items()) {
        // std::cout << item.key() << " : " << item.value() << std::endl;
        std::string email = item.value().at("email");
        std::cout << "Email: " << email << std::endl;
    }

    return 0;
}
