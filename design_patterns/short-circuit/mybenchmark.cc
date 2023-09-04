#include <benchmark/benchmark.h>

// An expensive condition that takes time to compute
bool expensive_condition(int i) {
  for (int j = 0; j < 1000000; ++j) {}  // mimic heavy computation
  return i % 2 == 0;
}

// A cheap condition that is very fast to compute
bool cheap_condition(int i) {
  return i % 2 != 0;
}

// Function that does not short circuit
static void BM_NoShortCircuit(benchmark::State& state) {
  for (auto _ : state) {
	bool result = false;
	for (int i = 0; i < state.range(0); ++i) {
  	result = expensive_condition(i) | cheap_condition(i);  // non-short-circuit
	}
	benchmark::DoNotOptimize(result);
  }
}
BENCHMARK(BM_NoShortCircuit)->Range(8, 8<<10);

// Function that uses short circuit
static void BM_ShortCircuit(benchmark::State& state) {
  for (auto _ : state) {
	bool result = false;
	for (int i = 0; i < state.range(0); ++i) {
  	result = cheap_condition(i) || expensive_condition(i);  // short-circuit
	}
	benchmark::DoNotOptimize(result);
  }
}
BENCHMARK(BM_ShortCircuit)->Range(8, 8<<10);

BENCHMARK_MAIN();
