#include <benchmark/benchmark.h>
#include <vector>

// Function without __builtin_prefetch
void NoPrefetch(benchmark::State& state) {
  // Create a large vector to iterate over
  std::vector<int> data(state.range(0), 1);
  for (auto _ : state) {
    long sum = 0;
    for (const auto& i : data) {
      sum += i;
    }
    // Prevent compiler optimization to discard the sum
    benchmark::DoNotOptimize(sum);
  }
}
BENCHMARK(NoPrefetch)->Arg(1<<20); // Run with 1MB of data (2^20 integers)


// Function with __builtin_prefetch
void WithPrefetch(benchmark::State& state) {
  // Create a large vector to iterate over
  std::vector<int> data(state.range(0), 1);
  for (auto _ : state) {
    long sum = 0;
    int prefetch_distance = 10;
    for (int i = 0; i < data.size(); i++) {
      if (i + prefetch_distance < data.size()) {
    	__builtin_prefetch(&data[i + prefetch_distance], 0, 3);
      }
      sum += data[i];
    }
    // Prevent compiler optimization to discard the sum
    benchmark::DoNotOptimize(sum);
  }
}
BENCHMARK(WithPrefetch)->Arg(1<<20); // Run with 1MB of data (2^20 integers)

BENCHMARK_MAIN();
