// EventProcessor.cpp

#include "EventProcessor.h"
#include <iostream>

EventProcessor::EventProcessor(std::shared_ptr<RingBuffer> ringBuffer, std::shared_ptr<Sequencer> sequencer)
    : ringBuffer_(ringBuffer), sequencer_(sequencer), nextSequence_(0), running_(true) {}

void EventProcessor::run() {
    while (running_) {
        while (nextSequence_ <= sequencer_->cursor()) {
            Event& event = ringBuffer_->get(nextSequence_);
            //std::cout << "[Consumer] Consumed: " << event.get() << " from sequence: " << nextSequence_ << "\n";
            ++nextSequence_;
        }
    }
}

void EventProcessor::stop() {
    running_ = false;
}

