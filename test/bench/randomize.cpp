#include <string>
#include <string_view>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <random>
#include <limits>

#include "experimentation/random.hpp"

#include <nlohmann/json.hpp>
#include <benchmark/benchmark.h>

template<typename T>
static void randomize_test(benchmark::State& state)
{
    const auto min = std::numeric_limits<T>::min();
    const auto max = std::numeric_limits<T>::max();

    auto size = state.range(0);

    T storage = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(storage);
        storage = experimentation::fast_randomize_64<T>(min, max);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(T));
}
BENCHMARK_TEMPLATE(randomize_test, uint64_t)->Name("randomize")->RangeMultiplier(16)->Range(256, 4096)->ThreadRange(1, 1);

template<typename T>
static void randomize_tp_test(benchmark::State& state)
{
    const auto min = std::numeric_limits<T>::min();
    const auto max = std::numeric_limits<T>::max();

    auto size = state.range(0);

    T storage = 0;
    for (auto _ : state) {
        benchmark::DoNotOptimize(storage);
        storage = experimentation::fast_randomize_tp_64<T, min, max>();
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(T));
}
BENCHMARK_TEMPLATE(randomize_tp_test, uint64_t)->Name("randomize_tp")->RangeMultiplier(16)->Range(256, 4096)->ThreadRange(1, 1);
