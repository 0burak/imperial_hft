// Disruptor.cpp

#include "Disruptor.h"

Disruptor::Disruptor(size_t bufferSize, std::vector<EventProcessor*>& processors, std::vector<Producer*>& producers, WaitStrategy* waitStrategy)
    : buffer(bufferSize), sequencer(buffer), processors(processors), producers(producers), waitStrategy(waitStrategy) {
    for (EventProcessor* processor : processors) {
        processor->setSequencer(&sequencer);
    }
}

Disruptor::~Disruptor() {
    halt();
    delete waitStrategy;
}

void Disruptor::start() {
    for (EventProcessor* processor : processors) {
        // In reality, you would want to start each processor running in a separate thread here
        processor->start();
    }
}

void Disruptor::halt() {
    for (EventProcessor* processor : processors) {
        processor->halt();
    }
}

