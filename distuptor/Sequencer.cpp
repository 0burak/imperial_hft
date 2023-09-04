// Sequencer.cpp

#include "Sequencer.h"

Sequencer::Sequencer() : cursor_(-1) {}

long Sequencer::next() {
    return ++cursor_;
}

void Sequencer::publish(long sequence) {
    cursor_ = sequence;
}

long Sequencer::cursor() const {
    return cursor_.load();
}

