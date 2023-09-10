#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>

constexpr int kSize = 10000000;  
std::vector<int> data(kSize);
std::vector<int> indices(kSize);

static void BM_CacheCold(benchmark::State& state) {
  // Generate random indices
  for(auto& index : indices) {
    index = rand() % kSize;
  }
  for (auto _ : state) {
    int sum = 0;
    // Access data in random order
    for (int i = 0; i < kSize; ++i) {
      benchmark::DoNotOptimize(sum += data[indices[i]]);
    }
    benchmark::ClobberMemory();
  }
}
//BENCHMARK(BM_CacheCold);

static void BM_CacheWarm(benchmark::State& state) {
  // Warm cache by accessing data in sequential order
  int sum_warm = 0;
  for (int i = 0; i < kSize; ++i) {
    benchmark::DoNotOptimize(sum_warm += data[i]);
  }
  benchmark::ClobberMemory();
 
  // Run the benchmark
  for (auto _ : state) {
    int sum = 0;
    // Access data in sequential order again
    for (int i = 0; i < kSize; ++i) {
      benchmark::DoNotOptimize(sum += data[i]);
    }
    benchmark::ClobberMemory();
  }
}
BENCHMARK(BM_CacheWarm);

BENCHMARK_MAIN();
