// EventProcessor.h

#ifndef EVENT_PROCESSOR_H
#define EVENT_PROCESSOR_H

#include "Sequencer.h"
#include "RingBuffer.h"

class EventProcessor {
public:
    EventProcessor(std::shared_ptr<RingBuffer> ringBuffer, std::shared_ptr<Sequencer> sequencer);
    void run();
    void stop();

private:
    std::shared_ptr<RingBuffer> ringBuffer_;
    std::shared_ptr<Sequencer> sequencer_;
    long nextSequence_;
    bool running_;
};

#endif  // EVENT_PROCESSOR_H

