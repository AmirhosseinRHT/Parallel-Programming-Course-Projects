#include "baker.h"
#include <unistd.h>
#include <iostream>
#include "oven.h"


void Baker::BakeOrder(pthread_mutex_t& ovenLock, pthread_cond_t& ovenCond , Oven * oven){
    while(orderCnt > 0){
        pthread_mutex_lock(&ovenLock);
        while(oven->getFreeSpace() == 0){
            pthread_cond_wait(&ovenCond, &ovenLock);
        }
        int currentSpace = std::min(orderCnt , oven->getFreeSpace());
        oven->addBreadToOven(orderName, currentSpace);
        pthread_mutex_unlock(&ovenLock);
        orderCnt -= currentSpace;
    }
    pthread_mutex_unlock(&ovenLock);
    
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
 