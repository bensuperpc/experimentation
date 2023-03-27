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

BENCHMARK_TEMPLATE(vector_add, uint8_t)->Name("vector_add_uint8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_add, uint_fast8_t)->Name("vector_add_uint_fast8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_add, uint16_t)->Name("vector_add_uint16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_add, uint_fast16_t)->Name("vector_add_uint_fast16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_add, uint32_t)->Name("vector_add_uint32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_add, uint_fast32_t)->Name("vector_add_uint_fast32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_add, uint64_t)->Name("vector_add_uint64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_add, uint_fast64_t)->Name("vector_add_uint_fast64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);

template<class T>
static void vector_sub(benchmark::State& state)
{
    size_t size = state.range(0);

    std::vector<T> vec1(size, static_cast<T>(1));
    std::vector<T> vec2(size, static_cast<T>(2));
    std::vector<T> vec3(size, static_cast<T>(0));

    for (auto _ : state) {
        for (size_t i = 0; i < size; ++i) {
            vec3[i] = vec1[i] - vec2[i];
        }
        benchmark::DoNotOptimize(vec1);
        benchmark::DoNotOptimize(vec2);
        benchmark::DoNotOptimize(vec3);
        benchmark::ClobberMemory();
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(T));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK_TEMPLATE(vector_sub, uint8_t)->Name("vector_sub_uint8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_sub, uint_fast8_t)->Name("vector_sub_uint_fast8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_sub, uint16_t)->Name("vector_sub_uint16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_sub, uint_fast16_t)->Name("vector_sub_uint_fast16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_sub, uint32_t)->Name("vector_sub_uint32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_sub, uint_fast32_t)->Name("vector_sub_uint_fast32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_sub, uint64_t)->Name("vector_sub_uint64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_sub, uint_fast64_t)->Name("vector_sub_uint_fast64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);

template<class T>
static void vector_mul(benchmark::State& state)
{
    size_t size = state.range(0);

    std::vector<T> vec1(size, static_cast<T>(1));
    std::vector<T> vec2(size, static_cast<T>(2));
    std::vector<T> vec3(size, static_cast<T>(0));

    for (auto _ : state) {
        for (size_t i = 0; i < size; ++i) {
            vec3[i] = vec1[i] * vec2[i];
        }
        benchmark::DoNotOptimize(vec1);
        benchmark::DoNotOptimize(vec2);
        benchmark::DoNotOptimize(vec3);
        benchmark::ClobberMemory();
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(T));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK_TEMPLATE(vector_mul, uint8_t)->Name("vector_mul_uint8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mul, uint_fast8_t)->Name("vector_mul_uint_fast8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mul, uint16_t)->Name("vector_mul_uint16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mul, uint_fast16_t)->Name("vector_mul_uint_fast16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mul, uint32_t)->Name("vector_mul_uint32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mul, uint_fast32_t)->Name("vector_mul_uint_fast32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mul, uint64_t)->Name("vector_mul_uint64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mul, uint_fast64_t)->Name("vector_mul_uint_fast64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);

template<class T>
static void vector_div(benchmark::State& state)
{
    size_t size = state.range(0);

    std::vector<T> vec1(size, static_cast<T>(1));
    std::vector<T> vec2(size, static_cast<T>(2));
    std::vector<T> vec3(size, static_cast<T>(0));

    for (auto _ : state) {
        for (size_t i = 0; i < size; ++i) {
            vec3[i] = vec1[i] / vec2[i];
        }
        benchmark::DoNotOptimize(vec1);
        benchmark::DoNotOptimize(vec2);
        benchmark::DoNotOptimize(vec3);
        benchmark::ClobberMemory();
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(T));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK_TEMPLATE(vector_div, uint8_t)->Name("vector_div_uint8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_div, uint_fast8_t)->Name("vector_div_uint_fast8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_div, uint16_t)->Name("vector_div_uint16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_div, uint_fast16_t)->Name("vector_div_uint_fast16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_div, uint32_t)->Name("vector_div_uint32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_div, uint_fast32_t)->Name("vector_div_uint_fast32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_div, uint64_t)->Name("vector_div_uint64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_div, uint_fast64_t)->Name("vector_div_uint_fast64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);

template<class T>
static void vector_mod(benchmark::State& state)
{
    size_t size = state.range(0);

    std::vector<T> vec1(size, static_cast<T>(1));
    std::vector<T> vec2(size, static_cast<T>(2));
    std::vector<T> vec3(size, static_cast<T>(0));

    for (auto _ : state) {
        for (size_t i = 0; i < size; ++i) {
            vec3[i] = vec1[i] % vec2[i];
        }
        benchmark::DoNotOptimize(vec1);
        benchmark::DoNotOptimize(vec2);
        benchmark::DoNotOptimize(vec3);
        benchmark::ClobberMemory();
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(T));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK_TEMPLATE(vector_mod, uint8_t)->Name("vector_mod_uint8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mod, uint_fast8_t)->Name("vector_mod_uint_fast8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mod, uint16_t)->Name("vector_mod_uint16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mod, uint_fast16_t)->Name("vector_mod_uint_fast16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mod, uint32_t)->Name("vector_mod_uint32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mod, uint_fast32_t)->Name("vector_mod_uint_fast32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mod, uint64_t)->Name("vector_mod_uint64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_mod, uint_fast64_t)->Name("vector_mod_uint_fast64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);

template<class T>
static void vector_and(benchmark::State& state)
{
    size_t size = state.range(0);

    std::vector<T> vec1(size, static_cast<T>(1));
    std::vector<T> vec2(size, static_cast<T>(2));
    std::vector<T> vec3(size, static_cast<T>(0));

    for (auto _ : state) {
        for (size_t i = 0; i < size; ++i) {
            vec3[i] = vec1[i] & vec2[i];
        }
        benchmark::DoNotOptimize(vec1);
        benchmark::DoNotOptimize(vec2);
        benchmark::DoNotOptimize(vec3);
        benchmark::ClobberMemory();
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(T));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK_TEMPLATE(vector_and, uint8_t)->Name("vector_and_uint8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_and, uint_fast8_t)->Name("vector_and_uint_fast8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_and, uint16_t)->Name("vector_and_uint16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_and, uint_fast16_t)->Name("vector_and_uint_fast16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_and, uint32_t)->Name("vector_and_uint32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_and, uint_fast32_t)->Name("vector_and_uint_fast32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_and, uint64_t)->Name("vector_and_uint64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_and, uint_fast64_t)->Name("vector_and_uint_fast64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);

template<class T>
static void vector_or(benchmark::State& state)
{
    size_t size = state.range(0);

    std::vector<T> vec1(size, static_cast<T>(1));
    std::vector<T> vec2(size, static_cast<T>(2));
    std::vector<T> vec3(size, static_cast<T>(0));

    for (auto _ : state) {
        for (size_t i = 0; i < size; ++i) {
            vec3[i] = vec1[i] | vec2[i];
        }
        benchmark::DoNotOptimize(vec1);
        benchmark::DoNotOptimize(vec2);
        benchmark::DoNotOptimize(vec3);
        benchmark::ClobberMemory();
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(T));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK_TEMPLATE(vector_or, uint8_t)->Name("vector_or_uint8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_or, uint_fast8_t)->Name("vector_or_uint_fast8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_or, uint16_t)->Name("vector_or_uint16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_or, uint_fast16_t)->Name("vector_or_uint_fast16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_or, uint32_t)->Name("vector_or_uint32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_or, uint_fast32_t)->Name("vector_or_uint_fast32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_or, uint64_t)->Name("vector_or_uint64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_or, uint_fast64_t)->Name("vector_or_uint_fast64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);

template<class T>
static void vector_xor(benchmark::State& state)
{
    size_t size = state.range(0);

    std::vector<T> vec1(size, static_cast<T>(1));
    std::vector<T> vec2(size, static_cast<T>(2));
    std::vector<T> vec3(size, static_cast<T>(0));

    for (auto _ : state) {
        for (size_t i = 0; i < size; ++i) {
            vec3[i] = vec1[i] ^ vec2[i];
        }
        benchmark::DoNotOptimize(vec1);
        benchmark::DoNotOptimize(vec2);
        benchmark::DoNotOptimize(vec3);
        benchmark::ClobberMemory();
    }
    state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(T));
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK_TEMPLATE(vector_xor, uint8_t)->Name("vector_xor_uint8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_xor, uint_fast8_t)->Name("vector_xor_uint_fast8_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_xor, uint16_t)->Name("vector_xor_uint16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_xor, uint_fast16_t)->Name("vector_xor_uint_fast16_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_xor, uint32_t)->Name("vector_xor_uint32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_xor, uint_fast32_t)->Name("vector_xor_uint_fast32_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_xor, uint64_t)->Name("vector_xor_uint64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);
BENCHMARK_TEMPLATE(vector_xor, uint_fast64_t)->Name("vector_xor_uint_fast64_t")->RangeMultiplier(32)->Range(1, 1048576)->ThreadRange(1, 8);

