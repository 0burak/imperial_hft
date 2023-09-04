// Sequencer.h

#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <atomic>

class Sequencer {
public:
    Sequencer();
    long next();
    void publish(long sequence);
    long cursor() const;

private:
    std::atomic<long> cursor_;
};

#endif  // SEQUENCER_H

