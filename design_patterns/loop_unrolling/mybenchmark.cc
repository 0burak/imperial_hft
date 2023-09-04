#include <benchmark/benchmark.h>
#include <string>
#include <vector>
using namespace std;


#include <benchmark/benchmark.h>

// Benchmark function without loop unrolling
static void BM_LoopWithoutUnrolling(benchmark::State& state) {
  for (auto _ : state) {
    int result = 0;
    for (int i = 0; i < state.range(0); ++i) {
      result += i;
    }
    benchmark::DoNotOptimize(result);
  }
}
BENCHMARK(BM_LoopWithoutUnrolling)->Arg(1000);

// Benchmark function with loop unrolling
static void BM_LoopWithUnrolling(benchmark::State& state) {
  for (auto _ : state) {
    int result = 0;
    for (int i = 0; i < state.range(0); i += 4) {
      result += i + (i + 1) + (i + 2) + (i + 3);
    }
    benchmark::DoNotOptimize(result);
  }
}
BENCHMARK(BM_LoopWithUnrolling)->Arg(1000);

BENCHMARK_MAIN();
