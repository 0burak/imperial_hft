#include "RingBuffer.h"
#include "Sequencer.h"
#include "Producer.h"
#include "EventProcessor.h"
#include "Barrier.h"
#include "Disruptor.h"
#include "YieldWaitStrategy.h"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <benchmark/benchmark.h>

static constexpr size_t kBufferSize = 1024;
static constexpr size_t NUM_EVENTS = 1000;

static void BM_Disruptor(benchmark::State& state) {
  for (auto _ : state) {

    state.PauseTiming(); // Don't include setup time

    auto ringBuffer = std::make_shared<RingBuffer>(kBufferSize);
    auto sequencer = std::make_shared<Sequencer>();

    Producer producer(ringBuffer, sequencer);
    EventProcessor consumer(ringBuffer, sequencer);

    std::vector<EventProcessor*> processors = {&consumer};
    std::vector<Producer*> producers = {&producer};

    Disruptor disruptor(kBufferSize, processors, producers, new YieldWaitStrategy());

    state.ResumeTiming();  // Start timing again
    
    disruptor.start();


    for (int i = 0; i < NUM_EVENTS; ++i) {
      producer.onData("Event " + std::to_string(i));
    }

    state.PauseTiming(); 
    disruptor.halt();
    state.ResumeTiming();
    
  }
}
BENCHMARK(BM_Disruptor);


std::queue<std::string> simpleQueue;
std::mutex queueMutex;
std::condition_variable queueCondVar;

void SimpleProducer() {
  for (int i = 0; i < NUM_EVENTS; ++i) {
    std::unique_lock<std::mutex> lock(queueMutex);
    simpleQueue.push("Event " + std::to_string(i));
    lock.unlock();
    queueCondVar.notify_one();
  }
}

void SimpleConsumer() {
  int processedEvents = 0;
  while (processedEvents < NUM_EVENTS) {
    std::unique_lock<std::mutex> lock(queueMutex);
    queueCondVar.wait(lock, [&]() { return !simpleQueue.empty(); });
    simpleQueue.pop();
    ++processedEvents;
    lock.unlock();
  }
}

static void BM_SimpleQueue(benchmark::State& state) {
  for (auto _ : state) {

    state.PauseTiming(); // Don't include setup time

    std::queue<std::string> empty;
    std::swap(simpleQueue, empty);

    std::thread producerThread(SimpleProducer);
    std::thread consumerThread(SimpleConsumer);

    state.ResumeTiming(); // Resume timing

    producerThread.join();
    consumerThread.join();

  }
}

BENCHMARK(BM_SimpleQueue);

BENCHMARK_MAIN();
