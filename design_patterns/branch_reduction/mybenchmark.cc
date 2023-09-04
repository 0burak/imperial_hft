#include <benchmark/benchmark.h>

// A typical error checking setup
int errorCounterA = 0;

bool checkForErrorA() {
  // Produce an error once every 10 calls
  errorCounterA++;
  return (errorCounterA % 10) == 0;
}

bool checkForErrorB() {
  // Simulate some error check
  return false;
}

bool checkForErrorC() {
  // Simulate some error check
  return false;
}

void handleErrorA() {
  // Simulate some error handling
}

void handleErrorB() {
  // Simulate some error handling
}

void handleErrorC() {
  // Simulate some error handling
}

void executeHotpath() {
  // Simulate some hot path execution
}

static void Branching(benchmark::State& state) {
  errorCounterA = 0;  // reset the counter before benchmark run
  for (auto _ : state) {
    if (checkForErrorA())
      handleErrorA();
    else if (checkForErrorB())
      handleErrorB();
    else if (checkForErrorC())
      handleErrorC();
    else
      executeHotpath();
  }
}

// A new setup using flags
enum ErrorFlags {
  ErrorA = 1 << 0,
  ErrorB = 1 << 1,
  ErrorC = 1 << 2,
  NoError = 0
};

int errorCounterFlags = 0;

ErrorFlags checkErrors() {
  // Produce ErrorA once every 10 calls
  errorCounterFlags++;
  return (errorCounterFlags % 10) == 0 ? ErrorA : NoError;
}

void HandleError(ErrorFlags errorFlags) {
  // Simulate some error handling based on flags
  if (errorFlags & ErrorA) {
    handleErrorA();
  }
  // handle other errors similarly...
}

void hotpath() {
  // Simulate some hot path execution
}

static void ReducedBranching(benchmark::State& state) {
  errorCounterFlags = 0;  // reset the counter before benchmark run
  for (auto _ : state) {
    ErrorFlags errorFlags = checkErrors();
    if (errorFlags)
      HandleError(errorFlags);
    else
      hotpath();
  }
}

// Register the functions as a benchmark
BENCHMARK(Branching);
BENCHMARK(ReducedBranching);

BENCHMARK_MAIN();
