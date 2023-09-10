// Producer.h

#ifndef PRODUCER_H
#define PRODUCER_H

#include "Sequencer.h"
#include "RingBuffer.h"

class Producer {
public:
    Producer(std::shared_ptr<RingBuffer> ringBuffer, std::shared_ptr<Sequencer> sequencer);
    void onData(const std::string& data);
    void setSequencer(std::shared_ptr<Sequencer> sequencer);

private:
    std::shared_ptr<RingBuffer> ringBuffer_;
    std::shared_ptr<Sequencer> sequencer_;
};

#endif  // PRODUCER_H

