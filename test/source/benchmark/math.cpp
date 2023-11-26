#include "experimentation/math.hpp"

#include <benchmark/benchmark.h>

static void schwarzschild_radius(benchmark::State& state)
{
    // Code inside this loop is measured repeatedly
    auto size = state.range(0);
    for (auto _ : state) {
        uint64_t radius {benlib::math::experimentation::schwarzschild_radius(size)};
        benchmark::DoNotOptimize(radius);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * state.range(0)
                            * sizeof(decltype(benlib::math::experimentation::schwarzschild_radius(size))));
}
BENCHMARK(schwarzschild_radius)->Name("schwarzschild_radius")->RangeMultiplier(16)->Range(1, 1048576);

int main(int argc, char** argv) {
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}
