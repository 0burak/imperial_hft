// WaitStrategy.cpp

#include "WaitStrategy.h"

long BusySpinWaitStrategy::waitFor(long sequence, Sequencer& cursor, Sequencer& dependentCursor, Barrier& barrier) {
    long availableSequence;

    while ((availableSequence = dependentCursor.getCursor()) < sequence) {
        // Busy spinning
    }

    return availableSequence;
}

