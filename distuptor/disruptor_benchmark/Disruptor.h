// Disruptor.h

#ifndef DISRUPTOR_H
#define DISRUPTOR_H

#include <vector>
#include "EventProcessor.h"
#include "Producer.h"
#include "RingBuffer.h"
#include "Sequencer.h"
#include "WaitStrategy.h"
#include <thread>

class Disruptor {
public:
    explicit Disruptor(size_t bufferSize, std::vector<EventProcessor*>& processors, std::vector<Producer*>& producers, WaitStrategy* waitStrategy);
    ~Disruptor();

    void start();
    void halt();
    std::vector<std::thread> threads_;

private:
    //RingBuffer buffer;
    std::shared_ptr<Sequencer> sequencer;  // Changed from Sequencer to std::shared_ptr<Sequencer>
    std::vector<EventProcessor*> processors;
    std::vector<Producer*> producers;
    WaitStrategy* waitStrategy;
    std::shared_ptr<RingBuffer> buffer;  // This should be shared
};

#endif // DISRUPTOR_H

