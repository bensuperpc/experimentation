#include "experimentation/fileOnDemand.hpp"

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iterator>

class image final : public fileOnDemand<std::vector<char>> {
    public:
        image(const std::filesystem::path &p) : fileOnDemand(p) {
            std::cout << "image" << std::endl;
        }

        image(const image&) = delete;
        image& operator=(const image&) = delete;

        image(image&& other) : fileOnDemand(std::move(other)) {
            std::cout << "image move" << std::endl;
        }

    void load() override {
        if (_data.has_value()) {
            std::cout << "File: " << _path << " is already loaded" << std::endl;
            return;
        }

        if (!std::filesystem::exists(_path)) {
            std::cout << "File: " << _path << " does not exist" << std::endl;
            return;
        }

        std::ifstream __file;

        __file.unsetf(std::ios::skipws);

        __file.exceptions(std::ifstream::badbit
            | std::ifstream::failbit
            | std::ifstream::eofbit);

        __file.open(_path, std::ifstream::in | std::ifstream::binary);
        std::cout << "File: " << _path << " is open" << std::endl;

        if (!__file.is_open() || __file.bad()) {
            std::cout << "File: " << _path << " is not open" << std::endl;
            return;
        }

        __file.seekg(0, std::ios::end);
        std::streampos length(__file.tellg());
        __file.seekg(0, std::ios::beg);

        std::vector<char> __data;
        
        __data.resize(static_cast<std::size_t>(length));
        __file.read(&__data.front(), static_cast<std::streamsize>(length));

        _data = std::move(__data);

        std::cout << "File: " << _path << " is loaded" << std::endl;
    }

    void unload() override {
        std::cout << "unload" << std::endl;
        _data.reset();
    }

    void write() {
        std::cout << "File: " << _path << " is writing" << std::endl;
        if (!_data.has_value()) {
            return;
        }

        std::ofstream __file;
        __file.exceptions(std::ofstream::badbit
            | std::ofstream::failbit
            | std::ofstream::eofbit);
        
        __file.open(_path, std::ofstream::out | std::ofstream::binary);
        __file.write(&_data.value().front(), static_cast<std::streamsize>(_data.value().size()));

        std::cout << "File: " << _path << " is written" << std::endl;
    }
};

int main(int argc, char **argv) {
    // Get the fileOnDemand path from the command line

    std::filesystem::path p = argv[1];
    image i(p);
    //i.load();
    std::cout << i.data().size() << " Ko" << std::endl;
    i.write();
    i.unload();
    return 0;
}