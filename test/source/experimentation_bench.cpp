#include "experimentation/experimentation.hpp"

#include <benchmark/benchmark.h>

static void basic1_bench(benchmark::State& state)
{
  // Code inside this loop is measured repeatedly
  auto size = state.range(0);

  std::string str = "";
  benchmark::DoNotOptimize(str);

  for (auto _ : state) {
    str = "experimentation";
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * state.range(0) * sizeof(decltype(str)));
}

BENCHMARK(basic1_bench)->Name("basic1_bench")->RangeMultiplier(16)->Range(1, 1048576);
