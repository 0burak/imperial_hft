// main.cpp

#include "RingBuffer.h"
#include "Sequencer.h"
#include "Producer.h"
#include "EventProcessor.h"
#include "Barrier.h"
#include "YieldWaitStrategy.h"
#include <thread>

#include <chrono>
// main.cpp

#include "Disruptor.h"
#include "Producer.h"
#include "EventProcessor.h"

#include <vector>
#include <chrono>

int main() {
    size_t bufferSize = 1024;

    // Create shared instances of the RingBuffer and Sequencer
    auto ringBuffer = std::make_shared<RingBuffer>(bufferSize);
    auto sequencer = std::make_shared<Sequencer>();

    // Create the producer and consumer
    Producer producer(ringBuffer, sequencer);
    EventProcessor consumer(ringBuffer, sequencer);

    std::vector<EventProcessor*> processors = {&consumer};
    std::vector<Producer*> producers = {&producer};

    // Create the Disruptor
    Disruptor disruptor(bufferSize, processors, producers, new YieldWaitStrategy());





    
    // Start the Disruptor
    disruptor.start();

    // Generate some data in the producer
    for (int i = 0; i < 100; ++i) {
        producer.onData("Event " + std::to_string(i));
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // slow down the producer a bit
    }

    // Stop the Disruptor
    disruptor.halt();

    return 0;
}
