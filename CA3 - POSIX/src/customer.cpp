#include "customer.h"
#include <pthread.h>
#include <unistd.h>


Customer::Customer(std::string _name, int _breadCnt) {
    name = _name;
    breadCnt = _breadCnt;
}

std::string Customer::getName() {
    return name;
}

int Customer::getBreadCnt() {
    return breadCnt;
}

void Customer::announceOrder(pthread_mutex_t& orderLock, pthread_cond_t& orderCond, struct Order*& currentOrder) {
    pthread_mutex_lock(&orderLock);
    currentOrder = new struct Order;
    currentOrder->name = name;
    currentOrder->breadCnt = breadCnt;
    sleep(1);
    pthread_cond_broadcast(&orderCond);
    pthread_mutex_unlock(&orderLock);
}

void Customer::waitForBread(pthread_mutex_t& orderLock, pthread_cond_t& orderCond, struct Order*& currentOrder) {
    pthread_mutex_lock(&orderLock);
    while (currentOrder != nullptr) {
        pthread_cond_wait(&orderCond, &orderLock);
    }
    pthread_mutex_unlock(&orderLock);
}
