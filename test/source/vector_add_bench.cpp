#include <algorithm>
#include <vector>

#include <benchmark/benchmark.h>

template<class T>
static void vector_add(benchmark::State& state)
{
    size_t size = state.range(0);

    std::vector<T> vec1(size, static_cast<T>(1));
    std::vector<T> vec2(size, static_cast<T>(2));
    std::vector<T> vec3(size, static_cast<T>(0));

    for (auto _ : state) {
        for (size_t i = 0; i < size; ++i) {
            vec3[i] = vec1[i] + vec2[i];
        }
        benchmark::DoNotOptimize(vec1);
        benchmark::DoNotOptimize(vec2);
        benchmark::DoNotOptimize(vec3);
        benchmark::ClobberMemory();
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(T));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

/*
BENCHMARK_TEMPLATE(vector_add, uint_fast8_t)->Name("vector_add_uint_fast8_t")->Arg(1048576);
*/

BENCHMARK_TEMPLATE(vector_add, uint8_t)->RangeMultiplier(32)->Range(1, 1048576);
BENCHMARK_TEMPLATE(vector_add, uint_fast8_t)->RangeMultiplier(32)->Range(1, 1048576);
BENCHMARK_TEMPLATE(vector_add, uint16_t)->RangeMultiplier(32)->Range(1, 1048576);
BENCHMARK_TEMPLATE(vector_add, uint_fast16_t)->RangeMultiplier(32)->Range(1, 1048576);
BENCHMARK_TEMPLATE(vector_add, uint32_t)->RangeMultiplier(32)->Range(1, 1048576);
BENCHMARK_TEMPLATE(vector_add, uint_fast32_t)->RangeMultiplier(32)->Range(1, 1048576);
BENCHMARK_TEMPLATE(vector_add, uint64_t)->RangeMultiplier(32)->Range(1, 1048576);
BENCHMARK_TEMPLATE(vector_add, uint_fast64_t)->RangeMultiplier(32)->Range(1, 1048576);