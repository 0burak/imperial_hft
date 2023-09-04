#include <benchmark/benchmark.h>
#include <atomic>
#include <mutex>

std::atomic<int> atomic_counter;
std::mutex counter_mutex;
int locked_counter;

void increment_atomic(int inc) {
  for (int i = 0; i < inc; ++i) {
    atomic_counter++;
  }
}

void increment_locked(int inc) {
  for (int i = 0; i < inc; ++i) {
    std::lock_guard<std::mutex> lock(counter_mutex);
    locked_counter++;
  }
}

static void BM_Atomic(benchmark::State& state) {
  for (auto _ : state) {
    increment_atomic(state.range(0));
  }
}
// Register the function as a benchmark
BENCHMARK(BM_Atomic)->Arg(10000);

static void BM_Mutex(benchmark::State& state) {
  for (auto _ : state) {
    increment_locked(state.range(0));
  }
}
BENCHMARK(BM_Mutex)->Arg(10000);

BENCHMARK_MAIN();
