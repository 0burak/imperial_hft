// Event.h

#ifndef EVENT_H
#define EVENT_H

#include <string>

class Event {
public:
    Event() = default;
    void set(const std::string& value);
    std::string get() const;

private:
    std::string value_;
};

#endif  // EVENT_H

