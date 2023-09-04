// YieldWaitStrategy.h

#ifndef YIELDWAITSTRATEGY_H
#define YIELDWAITSTRATEGY_H

#include "WaitStrategy.h"

class YieldWaitStrategy : public WaitStrategy {
public:
    void wait() override;
};

#endif  // YIELDWAITSTRATEGY_H

