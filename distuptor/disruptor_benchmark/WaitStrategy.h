// WaitStrategy.h

#ifndef WAITSTRATEGY_H
#define WAITSTRATEGY_H

class WaitStrategy {
public:
    virtual ~WaitStrategy() = default;
    virtual void wait() = 0;
};

#endif  // WAITSTRATEGY_H

