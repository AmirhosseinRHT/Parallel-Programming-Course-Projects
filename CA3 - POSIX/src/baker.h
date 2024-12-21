#ifndef BAKER_H
#define BAKER_H

#include <pthread.h>
#include <string>
#include "order.h"

class Baker {
public:
    void setCurrentOrder(struct Order*& _currentOrder);
    void waitForOrder(pthread_mutex_t& orderLock, pthread_cond_t& orderCond);

private:
    struct Order* currentOrder;
};

#endif 
