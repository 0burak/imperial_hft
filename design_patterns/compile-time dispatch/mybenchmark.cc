#include <benchmark/benchmark.h>

class Base {
public:
  virtual ~Base() {}
  virtual int function() const = 0;
};

class Derived1 : public Base {
public:
  int function() const override {
    return 1;
  }
};

class Derived2 : public Base {
public:
  int function() const override {
    return 2;
  }
};

// Benchmark for runtime dispatch
static void BM_RuntimeDispatch(benchmark::State& state) {
  Base* obj;
  if (state.range(0) == 1)
    obj = new Derived1;
  else
    obj = new Derived2;

  for (auto _ : state) {
    benchmark::DoNotOptimize(obj->function());
  }
    
  delete obj;
}
BENCHMARK(BM_RuntimeDispatch)->Arg(1)->Arg(2);

// Benchmark for compile-time dispatch
template <typename T>
static void BM_CompileTimeDispatch(benchmark::State& state) {
  T obj;
  for (auto _ : state) {
    benchmark::DoNotOptimize(obj.function());
  }
}
BENCHMARK_TEMPLATE(BM_CompileTimeDispatch, Derived1);
BENCHMARK_TEMPLATE(BM_CompileTimeDispatch, Derived2);

BENCHMARK_MAIN();


