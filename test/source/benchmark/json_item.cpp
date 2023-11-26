#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <iostream>

#include <nlohmann/json.hpp>
#include <benchmark/benchmark.h>


class json_item_cached
{
private:
    nlohmann::json _data;
public:
    json_item_cached() {}
    ~json_item_cached() {}
    void load(const std::filesystem::path& path)
    {
        std::ifstream file(path);
        assert(file.is_open());

        _data = nlohmann::json::parse(file);
        // file >> _data;
    }

    std::string get(std::string_view key) const
    {
        if (_data.find(key) == _data.end()) {
            return "";
        }
        return _data[key];
    }
};

class json_item_uncached
{
    private:
        std::string _path;
    public:
    json_item_uncached() {}
    ~json_item_uncached() {}

    void load(const std::filesystem::path& path)
    {   
        assert(std::filesystem::exists(path));

        _path = path;
    }

    std::string get(std::string_view key) const
    {
        nlohmann::json data;
        std::ifstream file(_path);

        assert(file.is_open());

        data = nlohmann::json::parse(file);
        //file >> data;
        
        if (data.find(key) == data.end()) {
            return "";
        }
        return data[key];
    }
};

template<class T>
static void json_item(benchmark::State& state)
{
    auto size = state.range(0);

    if (std::filesystem::exists("data.json")) {
        std::filesystem::remove("data.json");
    }
    
    nlohmann::json data;
    for (size_t i = 0; i < size; ++i) {
        data["key." + std::to_string(i)] = std::to_string(i);
    }
    std::ofstream file("data.json");
    file << std::setw(4) << data << std::endl;

    T item;
    item.load("data.json");

    for (auto _ : state) {
        benchmark::DoNotOptimize(item);
        std::string &&value = item.get("key.255");
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * state.range(0));
}
BENCHMARK_TEMPLATE(json_item, json_item_cached)->Name("cached_json")->RangeMultiplier(16)->Range(256, 4096)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(json_item, json_item_uncached)->Name("uncached_json")->RangeMultiplier(16)->Range(256, 4096)->ThreadRange(1, 1);

int main(int argc, char** argv) {
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}
