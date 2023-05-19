#include <benchmark/benchmark.h>

#include <algorithm>
#include <boost/container/flat_set.hpp>
#include <iostream>
#include <queue>
#include <random>
#include <stack>
#include <stdexcept>
#include <unordered_set>
#include <vector>
using namespace std;

template <typename RandomEngine>
std::vector<int> TestData(size_t size, RandomEngine& re) {
  std::uniform_int_distribution<int> gen(1, 10'000'000);
  vector<int> r(size);
  for (int& x : r) {
    x = gen(re);
  }
  return r;
}

template <typename Set>
void Find(benchmark::State& state) {
  std::default_random_engine re(20230518);
  const auto values = TestData(state.range(0), re);
  Set sud(values.begin(), values.end());

  std::uniform_int_distribution<size_t> take_absent(1, 100);
  for (auto _ : state) {
    int x = values.front();
    if (take_absent(re) > 50) {
      x = -x;
    }
    auto it = sud.find(x);
    benchmark::DoNotOptimize(it);
  }
  state.SetComplexityN(values.size());
}

// Register the function as a benchmark
BENCHMARK(Find<std::unordered_set<int>>)
    ->RangeMultiplier(2)
    ->Range(1, 1 << 17)
    ->Complexity(benchmark::o1);
BENCHMARK(Find<boost::container::flat_set<int>>)
    ->RangeMultiplier(2)
    ->Range(1, 1 << 17)
    ->Complexity(benchmark::oLogN);

BENCHMARK_MAIN();