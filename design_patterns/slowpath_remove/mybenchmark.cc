#include <benchmark/benchmark.h>
#include <map>
#include <string>
#include <iostream>
using namespace std;

static std::map<int, int> orders;
static bool okay = false;

static void sendToClient(const std::string& message) {
  // Simulate some time consuming operation.
  for (volatile int i = 0; i < 100000; ++i);
}

static void removeFromMap(int orderId, std::map<int, int>& orders) {
  orders.erase(orderId);
}

static void logMessage(const std::string& message) {
  // Simulate some time consuming operation.
  for (volatile int i = 0; i < 100000; ++i);
}

static void __attribute__ ((noinline)) HandleError() {
  sendToClient("Order failed");
  removeFromMap(1, orders);
  logMessage("Order failed");
}

// Bad scenario.
static void SlowpathNotRemoved(benchmark::State& state) {
  int counter = 0;
  for (auto _ : state) {
    okay = (++counter % 10 != 0); // Hotpath 90% of the time, slowpath 10% of the time.
    if (okay) {
      // Hotpath.
    } else {
      // Slow-path
      sendToClient("Order failed");
      removeFromMap(1, orders);
      logMessage("Order failed");
    }
  }
}

// Register the function as a benchmark.
BENCHMARK(SlowpathNotRemoved);

// Better scenario.
static void SlowpathRemoved(benchmark::State& state) {
  int counter = 0;
  for (auto _ : state) {
    okay = (++counter % 10 != 0); // Hotpath 90% of the time, slowpath 10% of the time.
    if (okay) {
      // Hotpath.
    } else {
      HandleError();
    }
  }
}

// Register the function as a benchmark.
BENCHMARK(SlowpathRemoved);

// Run the benchmark.
BENCHMARK_MAIN();
