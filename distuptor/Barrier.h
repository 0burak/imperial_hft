// Barrier.h

#ifndef BARRIER_H
#define BARRIER_H

#include <memory>
#include "Sequencer.h"

class Barrier {
public:
    Barrier(std::shared_ptr<Sequencer> sequencer);
    void waitFor(long sequence);

private:
    std::shared_ptr<Sequencer> sequencer_;
};

#endif  // BARRIER_H

