#include <benchmark/benchmark.h>
#include <cstdlib>

static void BM_MixedFloatDouble(benchmark::State& state) {
    for (auto _ : state) {
        float a, b = rand();
        a = b * 1.23;
    }
}
// Register the function as a benchmark
BENCHMARK(BM_MixedFloatDouble);

static void BM_UnmixedFloatDouble(benchmark::State& state) {
    for (auto _ : state) {
        float a, b = rand();
        a = b * 1.23f;
    }
}
BENCHMARK(BM_UnmixedFloatDouble);

BENCHMARK_MAIN();
