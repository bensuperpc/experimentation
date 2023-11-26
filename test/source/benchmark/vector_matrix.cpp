#include <algorithm>
#include <vector>

#include <benchmark/benchmark.h>

template<class T>
static void vector_1D(benchmark::State& state)
{
    size_t size = state.range(0);

    std::vector<T> vec1(size, static_cast<T>(1));

    for (auto _ : state) {
        for (size_t i = 0; i < size; ++i) {
            vec1[i] += static_cast<T>(1);
        }
        benchmark::DoNotOptimize(vec1);
        benchmark::ClobberMemory();
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(T));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

/*
BENCHMARK_TEMPLATE(vector_1D, uint_fast8_t)->Name("vector_1D_uint_fast8_t")->Arg(512);
*/

BENCHMARK_TEMPLATE(vector_1D, uint8_t)->Name("vector_1D_uint8_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_1D, uint_fast8_t)->Name("vector_1D_uint_fast8_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_1D, uint16_t)->Name("vector_1D_uint16_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_1D, uint_fast16_t)->Name("vector_1D_uint_fast16_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_1D, uint32_t)->Name("vector_1D_uint32_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_1D, uint_fast32_t)->Name("vector_1D_uint_fast32_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_1D, uint64_t)->Name("vector_1D_uint64_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_1D, uint_fast64_t)->Name("vector_1D_uint_fast64_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);

template<class T>
static void vector_2D(benchmark::State& state)
{
    size_t size = state.range(0);

    std::vector<std::vector<T>> vec1(size, std::vector<T>(size, static_cast<T>(1)));

    for (auto _ : state) {
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                vec1[i][j] += static_cast<T>(1);
            }
        }
        benchmark::DoNotOptimize(vec1);
        benchmark::ClobberMemory();
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(T));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK_TEMPLATE(vector_2D, uint8_t)->Name("vector_2D_uint8_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_2D, uint_fast8_t)->Name("vector_2D_uint_fast8_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_2D, uint16_t)->Name("vector_2D_uint16_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_2D, uint_fast16_t)->Name("vector_2D_uint_fast16_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_2D, uint32_t)->Name("vector_2D_uint32_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_2D, uint_fast32_t)->Name("vector_2D_uint_fast32_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_2D, uint64_t)->Name("vector_2D_uint64_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_2D, uint_fast64_t)->Name("vector_2D_uint_fast64_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);

template<class T>
static void vector_3D(benchmark::State& state)
{
    size_t size = state.range(0);

    std::vector<std::vector<std::vector<T>>> vec1(size, std::vector<std::vector<T>>(size, std::vector<T>(size, static_cast<T>(1))));

    for (auto _ : state) {
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                for (size_t k = 0; k < size; ++k) {
                    vec1[i][j][k] += static_cast<T>(1);
                }
            }
        }
        benchmark::DoNotOptimize(vec1);
        benchmark::ClobberMemory();
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(T));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK_TEMPLATE(vector_3D, uint8_t)->Name("vector_3D_uint8_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_3D, uint_fast8_t)->Name("vector_3D_uint_fast8_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_3D, uint16_t)->Name("vector_3D_uint16_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_3D, uint_fast16_t)->Name("vector_3D_uint_fast16_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_3D, uint32_t)->Name("vector_3D_uint32_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_3D, uint_fast32_t)->Name("vector_3D_uint_fast32_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_3D, uint64_t)->Name("vector_3D_uint64_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);
BENCHMARK_TEMPLATE(vector_3D, uint_fast64_t)->Name("vector_3D_uint_fast64_t")->RangeMultiplier(8)->Range(1, 512)->ThreadRange(1, 1);

int main(int argc, char** argv) {
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
}
