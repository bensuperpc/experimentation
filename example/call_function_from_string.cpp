#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "experimentation/class.hpp"

auto main() -> int
{
    benlib::object::experimentation::classes::Base experimentation;
    std::vector<std::pair<std::string, std::function<void(std::string_view)>>> list;

    std::vector<std::string> list_of_functions = {"function1", "function2", "function3", "function4"};

    list.emplace_back(std::make_pair(
        "function1",
        std::bind(&benlib::object::experimentation::classes::Base::function1, experimentation, std::placeholders::_1)));

    list.emplace_back(std::make_pair(
        "function2",
        std::bind(&benlib::object::experimentation::classes::Base::function2, experimentation, std::placeholders::_1)));

    list.emplace_back(std::make_pair(
        "function3",
        std::bind(&benlib::object::experimentation::classes::Base::function3, experimentation, std::placeholders::_1)));

    for (const auto& function : list_of_functions) {
        for (const auto& pair : list) {
            if (pair.first == function) {
                pair.second(function);
            }
        }
    }

    std::map<std::string, std::function<void(std::string_view)>> map;

    map.emplace(std::make_pair(
        "function1",
        std::bind(&benlib::object::experimentation::classes::Base::function1, experimentation, std::placeholders::_1)));

    map.emplace(std::make_pair(
        "function2",
        std::bind(&benlib::object::experimentation::classes::Base::function2, experimentation, std::placeholders::_1)));

    map.emplace(std::make_pair(
        "function3",
        std::bind(&benlib::object::experimentation::classes::Base::function3, experimentation, std::placeholders::_1)));

    for (const auto& function : list_of_functions) {
        if (map.find(function) != map.end()) {
            map[function](function);
        } else {
            std::cout << "Function not found: " << function << std::endl;
        }
    }

    /*
    for (const auto& [name, function] : map) {
      function(name);
    }
    */

    return 0;
}
