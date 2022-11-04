#include "experimentation/string.hpp"

#include <benchmark/benchmark.h>

static void string_is_palindrome_for(benchmark::State& state)
{
  // Code inside this loop is measured repeatedly
  auto size = state.range(0);

  std::string&& str = benlib::string::experimentation::random_string(size);

  for (auto _ : state) {
    benchmark::DoNotOptimize(benlib::string::experimentation::is_palindrome_for(str));
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * state.range(0)
                          * sizeof(decltype(benlib::string::experimentation::is_palindrome_for(str))));
}
BENCHMARK(string_is_palindrome_for)->Name("string_is_palindrome_for")->RangeMultiplier(16)->Range(1, 1048576);

static void string_is_palindrome_std_v1(benchmark::State& state)
{
  // Code inside this loop is measured repeatedly
  auto size = state.range(0);

  std::string&& str = benlib::string::experimentation::random_string(size);

  for (auto _ : state) {
    benchmark::DoNotOptimize(benlib::string::experimentation::is_palindrome_std_v1(str));
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * state.range(0)
                          * sizeof(decltype(benlib::string::experimentation::is_palindrome_std_v1(str))));
}
BENCHMARK(string_is_palindrome_std_v1)->Name("string_is_palindrome_std_v1")->RangeMultiplier(16)->Range(1, 1048576);

static void string_is_palindrome_std_v2(benchmark::State& state)
{
  // Code inside this loop is measured repeatedly
  auto size = state.range(0);

  std::string&& str = benlib::string::experimentation::random_string(size);

  for (auto _ : state) {
    benchmark::DoNotOptimize(benlib::string::experimentation::is_palindrome_std_v1(str));
    benchmark::ClobberMemory();
  }
  state.SetItemsProcessed(state.iterations());
  state.SetBytesProcessed(state.iterations() * state.range(0)
                          * sizeof(decltype(benlib::string::experimentation::is_palindrome_std_v2(str))));
}
BENCHMARK(string_is_palindrome_std_v2)->Name("string_is_palindrome_std_v2")->RangeMultiplier(16)->Range(1, 1048576);
