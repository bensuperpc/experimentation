#include "experimentation/file.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

class image final : public file<std::vector<char>> {
    public:
        image(const std::filesystem::path &p) : file(p) {
            std::cout << "image" << std::endl;
        }
    void load() override {
        std::cout << "load" << std::endl;

        if (_is_loaded) {
            return;
        }

        if (!std::filesystem::exists(_path)) {
            std::cout << "File does not exist" << std::endl;
            return;
        }

        std::ifstream file_;

        file_.exceptions(std::ifstream::badbit
            | std::ifstream::failbit
            | std::ifstream::eofbit);

        file_.open(_path, std::ifstream::in | std::ifstream::binary);
        file_.seekg(0, std::ios::end);

        std::streampos length(file_.tellg());

        if (length) {
            file_.seekg(0, std::ios::beg);
            _data.resize(static_cast<std::size_t>(length));
            file_.read(&_data.front(), static_cast<std::size_t>(length));
        }
        
        _is_loaded = true;

        std::cout << "Is loaded" << std::endl;  
    }

    void unload() override {
        std::cout << "unload" << std::endl;
        if (!_is_loaded) {
            return;
        }
        
        _is_loaded = false;
        _data.clear();
        _data.shrink_to_fit();
    }

    void write() {
        std::cout << "write" << std::endl;
        if (!_is_loaded) {
            return;
        }

        std::ofstream file_;
        file_.exceptions(std::ofstream::badbit
            | std::ofstream::failbit
            | std::ofstream::eofbit);
        
        file_.open(_path, std::ofstream::out | std::ofstream::binary);
        file_.write(&_data.front(), _data.size());

        std::cout << "Is written" << std::endl;
    }
};

int main(int argc, char **argv) {
    // Get the file path from the command line

    std::filesystem::path p = argv[1];
    image i(p);
    //i.load();
    std::cout << i.get_data().size() << " Ko" << std::endl;
    //i.write();
    i.unload();
    return 0;
}