#include "RingBuffer.h"
#include "Sequencer.h"
#include "Producer.h"
#include "EventProcessor.h"
#include "Barrier.h"
#include "YieldWaitStrategy.h"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <benchmark/benchmark.h>

constexpr int NUM_EVENTS = 1000000;

static void BM_Disruptor(benchmark::State& state) {
    for (auto _ : state) {
        // Create shared instances of the RingBuffer and Sequencer
        auto ringBuffer = std::make_shared<RingBuffer>(NUM_EVENTS);
        auto sequencer = std::make_shared<Sequencer>();

        // Create the producer and consumer
        Producer producer(ringBuffer, sequencer);
        EventProcessor consumer(ringBuffer, sequencer);
        std::thread consumerThread([&consumer]() { consumer.run(); });

        for (int i = 0; i < NUM_EVENTS; ++i) {
            producer.onData("Event " + std::to_string(i));
        }

        // Stop the consumer and wait for it to finish
        consumer.stop();
        consumerThread.join();
    }
}
BENCHMARK(BM_Disruptor);


static void BM_SimpleQueue(benchmark::State& state) {
    for (auto _ : state) {
        std::queue<std::string> queue; 
        std::mutex mutex;
        std::condition_variable condVar;
        bool done = false;

        std::thread consumerThread([&] {
            while (!done) {
                std::unique_lock<std::mutex> lock(mutex);
                while (queue.empty() && !done) {
                    condVar.wait(lock);
                }
                if (!queue.empty()) {
                    std::string event = queue.front(); 
                    queue.pop();
                }
            }
        });

        for (int i = 0; i < NUM_EVENTS; ++i) {
            std::string event = "Event " + std::to_string(i); 
            {
                std::lock_guard<std::mutex> lock(mutex);
                queue.push(event);
            }
            condVar.notify_one();
        }

        done = true;
        condVar.notify_one();
        consumerThread.join();
    }
}
BENCHMARK(BM_SimpleQueue);


BENCHMARK_MAIN();
