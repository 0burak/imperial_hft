// Disruptor.cpp

#include "Disruptor.h"
#include <thread>

Disruptor::Disruptor(size_t bufferSize, std::vector<EventProcessor*>& processors, std::vector<Producer*>& producers, WaitStrategy* waitStrategy)
    : buffer(std::make_shared<RingBuffer>(bufferSize)), sequencer(std::make_shared<Sequencer>()), processors(processors), producers(producers), waitStrategy(waitStrategy) {
    for (EventProcessor* processor : processors) {
        processor->setSequencer(sequencer);
    }

    for (Producer* producer : producers) {
        producer->setSequencer(sequencer);
    }
}


Disruptor::~Disruptor() {
    halt();
    for (auto& t : threads_) {
        if (t.joinable()) {
            t.join();
        }
    }
    delete waitStrategy;
}

void Disruptor::start() {
     for (EventProcessor* processor : processors) {
        threads_.emplace_back([processor]() { processor->run(); });
    }

    // Optionally, detach threads if you want
    for (auto& t : threads_) {
        t.detach();
    }
}

void Disruptor::halt() {
    for (EventProcessor* processor : processors) {
        processor->halt();
    }
}

