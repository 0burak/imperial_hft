// Disruptor.h

#ifndef DISRUPTOR_H
#define DISRUPTOR_H

#include <vector>
#include "EventProcessor.h"
#include "Producer.h"
#include "RingBuffer.h"
#include "Sequencer.h"
#include "WaitStrategy.h"

class Disruptor {
public:
    explicit Disruptor(size_t bufferSize, std::vector<EventProcessor*>& processors, std::vector<Producer*>& producers, WaitStrategy* waitStrategy);
    ~Disruptor();

    void start();
    void halt();

private:
    RingBuffer buffer;
    Sequencer sequencer;
    std::vector<EventProcessor*> processors;
    std::vector<Producer*> producers;
    WaitStrategy* waitStrategy;
};

#endif // DISRUPTOR_H

