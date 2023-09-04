// Event.cpp

#include "Event.h"

void Event::set(const std::string& value) {
    value_ = value;
}

std::string Event::get() const {
    return value_;
}

