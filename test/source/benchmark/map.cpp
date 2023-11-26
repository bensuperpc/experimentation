#include <unordered_map>
#include <map>
#include <string>

#include <benchmark/benchmark.h>

static void DoSetup([[maybe_unused]] const benchmark::State& state) {}

static void DoTeardown([[maybe_unused]] const benchmark::State& state) {}

static void map(benchmark::State& state)
{
    auto size = state.range(0);

    std::unordered_map<size_t, std::string> map;

    for (size_t i = 0; i < size; ++i) {
        map.emplace(i, std::to_string(i));
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(map);

        if (map.find(size) != map.end()) {
            benchmark::DoNotOptimize(map[size]);
        }
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * state.range(0) * map.size());
}
BENCHMARK(map)->Name("map")->RangeMultiplier(16)->Range(1, 1048576)->ThreadRange(1, 1);

static void unordered_map(benchmark::State& state)
{
    auto size = state.range(0);

    std::unordered_map<size_t, std::string> map;
    for (size_t i = 0; i < size; ++i) {
        map.emplace(i, std::to_string(i));
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(map);

        if (map.find(size) != map.end()) {
            benchmark::DoNotOptimize(map[size]);
        }
        benchmark::ClobberMemory();
        // state.PauseTiming();
        // state.SkipWithError("No path found");
        // state.ResumeTiming();

    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * state.range(0) * map.size());
    // state.counters["range"] = range;
    // state.SetLabel("OK");

}

BENCHMARK(unordered_map)
    ->Name("unordered_map")
    ->RangeMultiplier(16)
    ->Range(1, 1048576)
    ->ThreadRange(1, 1)
    ->Unit(benchmark::kNanosecond)
    ->Setup(DoSetup)
    ->Teardown(DoTeardown)
    ->MeasureProcessCPUTime()
    ->UseRealTime();



int main(int argc, char** argv) {
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}
