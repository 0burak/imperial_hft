// main.cpp

#include "RingBuffer.h"
#include "Sequencer.h"
#include "Producer.h"
#include "EventProcessor.h"
#include "Barrier.h"
#include "YieldWaitStrategy.h"
#include <thread>

#include <chrono>

int main() {
    // Create shared instances of the RingBuffer and Sequencer
    auto ringBuffer = std::make_shared<RingBuffer>(1024);
    auto sequencer = std::make_shared<Sequencer>();

    // Create the producer and consumer
    Producer producer(ringBuffer, sequencer);
    EventProcessor consumer(ringBuffer, sequencer);

    // Run the consumer in a separate thread
    std::thread consumerThread([&consumer]() { consumer.run(); });

    // Generate some data in the producer
    for (int i = 0; i < 1000; ++i) {
        producer.onData("Event " + std::to_string(i));
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // slow down the producer a bit
    }

    // In a real application, we would join the consumer thread here and perform cleanup
    // For this simple example, we'll just sleep for a while to make sure the consumer can finish
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
        // Stop the consumer and wait for it to finish
    consumer.stop();
    consumerThread.join();

    return 0;
}

