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

std::vector<int> TestData(size_t size) {
  vector<int> r(size);
  for (int& x : r) {
    x = rand();
  }
  return r;
}

template <typename Set> void SetFind(benchmark::State& state) {
  auto values = TestData(state.range(0));
  Set sud(values.begin(), values.end());

  std::default_random_engine re(20230518);
  std::uniform_int_distribution<int> take(0, 100);
  std::uniform_int_distribution<int> gen_random(-1000000, 1000000);

  for (auto _ : state) {
    for (int x : values) {
      if (take(re) < 50) {
        x = gen_random(re);
      }
      auto it = sud.find(x);
      benchmark::DoNotOptimize(it);
    }
  }
  state.SetComplexityN(values.size());
}

// Register the function as a benchmark
BENCHMARK(SetFind<std::unordered_set<int>>)
    ->RangeMultiplier(2)
    ->Range(1, 1 << 17)
    ->Complexity(benchmark::oN);
BENCHMARK(SetFind<boost::container::flat_set<int>>)
    ->RangeMultiplier(2)
    ->Range(1, 1 << 17)
    ->Complexity(benchmark::oNLogN);

BENCHMARK_MAIN();