// YieldWaitStrategy.cpp

#include "YieldWaitStrategy.h"
#include <thread>

void YieldWaitStrategy::wait() {
    std::this_thread::yield();
}

