#include <benchmark/benchmark.h>

int add(int a, int b) {
	return a + b;
}

__attribute__((always_inline)) int add_inline(int a, int b) {
	return a + b;
}

static void BM_WithoutInline(benchmark::State& state) {
	for (auto _ : state) {
    	benchmark::DoNotOptimize(add(5, 6));
	}
}
BENCHMARK(BM_WithoutInline);

static void BM_WithInline(benchmark::State& state) {
	for (auto _ : state) {
    	benchmark::DoNotOptimize(add_inline(5, 6));
	}
}
BENCHMARK(BM_WithInline);



BENCHMARK_MAIN();
