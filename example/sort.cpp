#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <vector>

template<typename T>
void print(std::string_view txt, const std::vector<T>& vec)
{
  std::cout << txt << ": ";
  for (const auto& e : vec) {
    std::cout << e << ' ';
  }
  std::cout << std::endl;
}

template<typename T>
void sort(std::vector<T>& vec)
{
  for (size_t i = 0; i < vec.size(); ++i) {
    for (size_t j = i + 1; j < vec.size(); ++j) {
      if (vec[i] > vec[j]) {
        std::swap(vec[i], vec[j]);
      }
    }
  }
}

template<typename T>
bool less(const T& a, const T& b)
{
  return a < b;
}

auto main() -> int
{
  std::vector<int> list {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto rd = std::default_random_engine {};
  auto rng = std::mt19937 {rd()};
  std::shuffle(list.begin(), list.end(), rng);

  print<int>("before: ", list);

  std::sort(list.begin(), list.end());

  print<int>("after:  ", list);

  std::shuffle(list.begin(), list.end(), rng);

  print<int>("before: ", list);

  std::sort(list.begin(), list.end(), std::less<int>());

  print<int>("after:  ", list);

  std::shuffle(list.begin(), list.end(), rng);

  print<int>("before: ", list);

  std::sort(list.begin(), list.end(), less<int>);

  print<int>("after:  ", list);

  std::shuffle(list.begin(), list.end(), rng);

  print<int>("before: ", list);

  std::sort(list.begin(), list.end(), [](int& a, int& b) { return a < b; });

  print<int>("after:  ", list);

  return 0;
}
