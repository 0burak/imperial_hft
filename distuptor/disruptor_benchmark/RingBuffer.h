// RingBuffer.h

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>
#include <memory>
#include "Event.h"

class RingBuffer {
public:
    RingBuffer(size_t bufferSize);
    Event& get(long sequence);
    long next();

private:
    std::vector<Event> buffer_;
    long nextSequence_;
};

#endif  // RINGBUFFER_H

