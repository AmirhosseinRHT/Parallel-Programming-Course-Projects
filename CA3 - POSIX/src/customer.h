#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <pthread.h>
#include "order.h" 
class Customer {
public:
    Customer(std::string _name, int _breadCnt);
    std::string getName();
    int getBreadCnt();

    void announceOrder(pthread_mutex_t& orderLock, pthread_cond_t& orderCond, struct Order*& currentOrder);
    void waitForBread(pthread_mutex_t& orderLock, pthread_cond_t& orderCond, struct Order*& currentOrder);

private:
    std::string name;
    int breadCnt;
};

#endif
