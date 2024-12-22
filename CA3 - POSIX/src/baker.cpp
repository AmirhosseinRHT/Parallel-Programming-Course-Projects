#include "baker.h"
#include <unistd.h>
#include <iostream>


void Baker::BakeOrder(pthread_mutex_t& ovenLock, pthread_cond_t& ovenCond){
    pthread_mutex_lock(&ovenLock);
    
}

void Baker::waitForOrder(pthread_mutex_t& orderLock, pthread_cond_t& orderCond, struct Order*& currentOrder) {
    pthread_mutex_lock(&orderLock);
    while (currentOrder == nullptr) {
        pthread_cond_wait(&orderCond, &orderLock);
        std::cout <<"baker waiting!" <<std::endl;
        if( currentOrder == nullptr) {
            std::cout <<"still nothing!" <<std::endl;
        }
    }
    orderName = currentOrder->name;
    orderCnt = currentOrder->breadCnt;
    std::cout << "Baking for: " << orderName << std::endl;
    sleep(1);
    // Implement baking
    free(currentOrder);
    currentOrder = nullptr;
    pthread_cond_broadcast(&orderCond);
    pthread_mutex_unlock(&orderLock);
    
}
 