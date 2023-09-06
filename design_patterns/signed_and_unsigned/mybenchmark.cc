#include <benchmark/benchmark.h>
#include <cstdlib>  // for rand()


// Function using signed integer
int f_signed(int i) {
    int j = 0;
    for (signed k = i; k < i + 10; ++k) {
        ++j;
    }
    return j;
}

// Function using unsigned integer
int f_unsigned(int i) {
    int j = 0;
    for (unsigned k = i; k < i + 10; ++k) {
        ++j;
    }
    return j;
}
// Benchmark for function using signed integer
static void BM_Signed(benchmark::State& state) {
    for (auto _ : state) {
        int i = rand();
        volatile int result = f_signed(i);
    }
}

// Benchmark for function using unsigned integer
static void BM_Unsigned(benchmark::State& state) {
    for (auto _ : state) {
        int i = rand();
        volatile int result = f_unsigned(i);
    }
}


// Register the benchmark functions
BENCHMARK(BM_Signed);
BENCHMARK(BM_Unsigned);

BENCHMARK_MAIN();
