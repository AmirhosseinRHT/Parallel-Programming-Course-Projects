#ifndef BAKER_H
#define BAKER_H

#include <pthread.h>
#include <string>
#include "order.h"

class Baker {
public:
    void waitForOrder(pthread_mutex_t& orderLock, pthread_cond_t& orderCond, struct Order*& currentOrder);

private:
};

#endif 
