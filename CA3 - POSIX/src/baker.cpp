#include "baker.h"
#include <unistd.h>
#include <iostream>

void Baker::setCurrentOrder(struct Order*& _currentOrder) {
    currentOrder = _currentOrder;
}

void Baker::waitForOrder(pthread_mutex_t& orderLock, pthread_cond_t& orderCond) {
    pthread_mutex_lock(&orderLock);
    while (currentOrder == nullptr) {
        pthread_cond_wait(&orderCond, &orderLock);
    }
    std::string orderName = currentOrder->name;
    int breadCnt = currentOrder->breadCnt;
    pthread_mutex_unlock(&orderLock);
    std::cout << "Baking for: " << orderName << std::endl;
    sleep(1);
    // Implement baking
    free(currentOrder);
    currentOrder = nullptr;
}
