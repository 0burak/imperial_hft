// Barrier.cpp

#include "Barrier.h"
#include <thread>

Barrier::Barrier(std::shared_ptr<Sequencer> sequencer) : sequencer_(sequencer) {}

void Barrier::waitFor(long sequence) {
    while (sequence > sequencer_->cursor()) {
        std::this_thread::yield();
    }
}

