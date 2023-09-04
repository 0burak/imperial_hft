#include <benchmark/benchmark.h>

// Function without optimization hints
int SquareIfPositiveNoHint(int x) {
    if (x > 0) {
        return x * x;
    } else {
        return 0;
    }
}

// Function with optimization hints
int SquareIfPositiveHint(int x) {
    if (__builtin_expect(x > 0, 1)) {
        return x * x;
    } else {
        return 0;
    }
}

// Benchmarks
static void BM_SquareIfPositiveNoHint(benchmark::State& state) {
    int x = state.range(0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(&x);
        SquareIfPositiveNoHint(x);
    }
}
BENCHMARK(BM_SquareIfPositiveNoHint)->Arg(10);

static void BM_SquareIfPositiveHint(benchmark::State& state) {
    int x = state.range(0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(&x);
        SquareIfPositiveHint(x);
    }
}
BENCHMARK(BM_SquareIfPositiveHint)->Arg(10);

BENCHMARK_MAIN();
