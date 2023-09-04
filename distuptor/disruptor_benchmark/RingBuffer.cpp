// RingBuffer.cpp

#include "RingBuffer.h"

RingBuffer::RingBuffer(size_t bufferSize)
    : buffer_(bufferSize), nextSequence_(0) {}

Event& RingBuffer::get(long sequence) {
    return buffer_[sequence % buffer_.size()];
}

long RingBuffer::next() {
    return nextSequence_++;
}

