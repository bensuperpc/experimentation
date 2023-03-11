#include <array>  // std::array
#include <functional>  // std::mem_fn, std::function
#include <string>  // std::string
#include <string_view>  // std::string_view
#include <vector>  // std::vector

#include <benchmark/benchmark.h>

class A
{
    public:
        A() {}
        ~A() {}
        __attribute__((noinline)) void set(const uint64_t i) { m_i = i; }
        __attribute__((noinline)) uint64_t get() { return m_i; }

    private:
        volatile uint64_t m_i;
};

static void ClassFnDirect(benchmark::State& state)
{
    auto range = state.range(0);

    A tmp;

    for (auto _ : state) {
        benchmark::DoNotOptimize(tmp);
        tmp.set(1);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(uint64_t));
}
BENCHMARK(ClassFnDirect)->Name("ClassFnDirect")->RangeMultiplier(10)->Range(1, 1)->ThreadRange(1, 1);
// BENCHMARK(_bench)->Name("_bench")->RangeMultiplier(10)->Range(1, 10000)->ThreadRange(1, 1)->UseRealTime();
// BENCHMARK(_bench)->Name("_bench")->DenseRange(0, 10000, 10)->ThreadRange(1, 1);

static void ClassFnPtr(benchmark::State& state)
{
    auto range = state.range(0);

    A tmp;

    // std::vector<void (A::*)(uint64_t i)> f_ptr
    constexpr void (A::*fn)(const uint64_t i) = &A::set;

    for (auto _ : state) {
        benchmark::DoNotOptimize(tmp);
        benchmark::DoNotOptimize(fn);
        (tmp.*fn)(1);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(uint64_t));
}
BENCHMARK(ClassFnPtr)->Name("ClassFnPtr")->RangeMultiplier(10)->Range(1, 1)->ThreadRange(1, 1);

template<typename T>
constexpr void call_set(T& obj, const uint64_t i)
{
    constexpr void (T::*fn)(const uint64_t i) = &T::set;
    (obj.*fn)(i);
}

static void ClassFnTemplate(benchmark::State& state)
{
    auto range = state.range(0);

    A tmp;

    for (auto _ : state) {
        benchmark::DoNotOptimize(tmp);
        call_set(tmp, 1);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(uint64_t));
}
BENCHMARK(ClassFnTemplate)->Name("ClassFnTemplate")->RangeMultiplier(10)->Range(1, 1)->ThreadRange(1, 1);

static void ClassFnFunction(benchmark::State& state)
{
    auto range = state.range(0);

    A tmp;

    // std::vector<std::function<void(A*, uint64_t)>> fn_ptr
    const std::function<void(A*, const uint64_t)> fn = std::mem_fn(&A::set);

    for (auto _ : state) {
        benchmark::DoNotOptimize(tmp);
        benchmark::DoNotOptimize(fn);
        fn(&tmp, 2);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(uint64_t));
}
BENCHMARK(ClassFnFunction)->Name("ClassFnFunction")->RangeMultiplier(10)->Range(1, 1)->ThreadRange(1, 1);

static void ClassFnLambda(benchmark::State& state)
{
    auto range = state.range(0);

    A tmp;

    const std::function<void(A*, const uint64_t)> fn = [](A* a, const uint64_t i) { a->set(i); };

    for (auto _ : state) {
        benchmark::DoNotOptimize(tmp);
        benchmark::DoNotOptimize(fn);
        fn(&tmp, 2);
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(uint64_t));
}
BENCHMARK(ClassFnLambda)->Name("ClassFnLambda")->RangeMultiplier(10)->Range(1, 1)->ThreadRange(1, 1);

static void ClassFnBind(benchmark::State& state)
{
    auto range = state.range(0);

    A tmp;

    const std::function<void(A*, const uint64_t)> fn = std::bind(&A::set, std::placeholders::_1, std::placeholders::_2);

    for (auto _ : state) {
        benchmark::DoNotOptimize(tmp);
        benchmark::DoNotOptimize(fn);
        fn(&tmp, 2);

        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(uint64_t));
}
BENCHMARK(ClassFnBind)->Name("ClassFnBind")->RangeMultiplier(10)->Range(1, 1)->ThreadRange(1, 1);
