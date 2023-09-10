// Producer.cpp

#include "Producer.h"
#include <iostream>

Producer::Producer(std::shared_ptr<RingBuffer> ringBuffer, std::shared_ptr<Sequencer> sequencer)
    : ringBuffer_(ringBuffer), sequencer_(sequencer) {    std::cout << "Producer Sequencer Addr: " << sequencer_.get() << std::endl;}

void Producer::onData(const std::string& data) {
    long sequence = sequencer_->next();
    Event& event = ringBuffer_->get(sequence);
    event.set(data);
    sequencer_->publish(sequence);
    std::cout << "[Producer] Published: " << data << " at sequence: " << sequence << "\n";

}


void Producer::setSequencer(std::shared_ptr<Sequencer> sequencer) {
    sequencer_ = sequencer;
}
