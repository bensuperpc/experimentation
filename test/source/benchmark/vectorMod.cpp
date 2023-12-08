#include <vector>

#include <benchmark/benchmark.h>

static constexpr int64_t multiplier = 2;
static constexpr int64_t minRange = 1;
static constexpr int64_t maxRange = 1048576;
static constexpr int64_t minThreadRange = 1;
static constexpr int64_t maxThreadRange = 1;

static void DoSetup([[maybe_unused]] const benchmark::State& state) {}

static void DoTeardown([[maybe_unused]] const benchmark::State& state) {}

template<class T>
static void vectorMod(benchmark::State& state)
{
    size_t size = state.range(0);

    std::vector<T> vec1(size, static_cast<T>(1));
    std::vector<T> vec2(size, static_cast<T>(2));
    std::vector<T> vec3(size, static_cast<T>(0));
    benchmark::ClobberMemory();

    for (auto _ : state) {
        for (size_t i = 0; i < size; ++i) {
            vec3[i] = vec1[i] % vec2[i];
        }
        //benchmark::DoNotOptimize(vec1);
        //benchmark::DoNotOptimize(vec2);
        //benchmark::DoNotOptimize(vec3);
        //benchmark::ClobberMemory();
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(T));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK(vectorMod<uint8_t>)
    ->Name("vectorMod<uint8_t>")
    ->RangeMultiplier(multiplier) 
    ->Range(minRange, maxRange) 
    ->ThreadRange(minThreadRange, maxThreadRange)
    ->Unit(benchmark::kNanosecond) 
    ->Setup(DoSetup) 
    ->Teardown(DoTeardown) 
    ->MeasureProcessCPUTime()
    ->UseRealTime()
    ->Repetitions(1);
BENCHMARK(vectorMod<uint_fast8_t>)
    ->Name("vectorMod<uint_fast8_t>")
    ->RangeMultiplier(multiplier) 
    ->Range(minRange, maxRange) 
    ->ThreadRange(minThreadRange, maxThreadRange)
    ->Unit(benchmark::kNanosecond)
    ->Setup(DoSetup) 
    ->Teardown(DoTeardown) 
    ->MeasureProcessCPUTime()
    ->UseRealTime()
    ->Repetitions(1);
BENCHMARK(vectorMod<uint16_t>)
    ->Name("vectorMod<uint16_t>")
    ->RangeMultiplier(multiplier)
    ->Range(minRange, maxRange)
    ->ThreadRange(minThreadRange, maxThreadRange) 
    ->Unit(benchmark::kNanosecond) 
    ->Setup(DoSetup)
    ->Teardown(DoTeardown) 
    ->MeasureProcessCPUTime()
    ->UseRealTime()
    ->Repetitions(1);
BENCHMARK(vectorMod<uint_fast16_t>)
    ->Name("vectorMod<uint_fast16_t>")
    ->RangeMultiplier(multiplier)
    ->Range(minRange, maxRange)
    ->ThreadRange(minThreadRange, maxThreadRange) 
    ->Unit(benchmark::kNanosecond) 
    ->Setup(DoSetup)
    ->Teardown(DoTeardown) 
    ->MeasureProcessCPUTime()
    ->UseRealTime()
    ->Repetitions(1);
BENCHMARK(vectorMod<uint32_t>)
    ->Name("vectorMod<uint32_t>")
    ->RangeMultiplier(multiplier)
    ->Range(minRange, maxRange)
    ->ThreadRange(minThreadRange, maxThreadRange) 
    ->Unit(benchmark::kNanosecond) 
    ->Setup(DoSetup)
    ->Teardown(DoTeardown) 
    ->MeasureProcessCPUTime()
    ->UseRealTime()
    ->Repetitions(1);
BENCHMARK(vectorMod<uint_fast32_t>)
    ->Name("vectorMod<uint_fast32_t>")
    ->RangeMultiplier(multiplier)
    ->Range(minRange, maxRange)
    ->ThreadRange(minThreadRange, maxThreadRange) 
    ->Unit(benchmark::kNanosecond) 
    ->Setup(DoSetup)
    ->Teardown(DoTeardown) 
    ->MeasureProcessCPUTime()
    ->UseRealTime()
    ->Repetitions(1);
BENCHMARK(vectorMod<uint64_t>)
    ->Name("vectorMod<uint64_t>")
    ->RangeMultiplier(multiplier)
    ->Range(minRange, maxRange)
    ->ThreadRange(minThreadRange, maxThreadRange) 
    ->Unit(benchmark::kNanosecond) 
    ->Setup(DoSetup)
    ->Teardown(DoTeardown) 
    ->MeasureProcessCPUTime()
    ->UseRealTime()
    ->Repetitions(1);
BENCHMARK(vectorMod<uint_fast64_t>)
    ->Name("vectorMod<uint_fast64_t>")
    ->RangeMultiplier(multiplier)
    ->Range(minRange, maxRange)
    ->ThreadRange(minThreadRange, maxThreadRange)
    ->Unit(benchmark::kNanosecond)
    ->Setup(DoSetup)
    ->Teardown(DoTeardown)
    ->MeasureProcessCPUTime()
    ->UseRealTime()
    ->Repetitions(1);

int main(int argc, char** argv) {
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}
