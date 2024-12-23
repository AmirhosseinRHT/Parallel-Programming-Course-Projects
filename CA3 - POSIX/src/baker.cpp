class Baker
{

public:
private:
};
#include "baker.h"
#include <unistd.h>
#include <iostream>
#include "oven.h"

void Baker::BakeOrder(pthread_mutex_t &ovenLock, pthread_cond_t &ovenCond, Oven *oven)
{
    pthread_mutex_lock(&ovenLock);
    while (orderCnt > 0)
    {
        if (oven->getFreeSpace() > 0)
        {
            int currentSpace = std::min(orderCnt, oven->getFreeSpace());
            oven->addBreadToOven(orderName, currentSpace);
            orderCnt -= currentSpace;
        }
        pthread_mutex_unlock(&ovenLock);
        sleep(2);
        pthread_mutex_lock(&ovenLock);
        oven->recieveBread(orderName);
    }
    pthread_mutex_unlock(&ovenLock);
}

void Baker::waitForOrder(pthread_mutex_t &orderLock, pthread_cond_t &orderCond, struct Order *&currentOrder)
{
    pthread_mutex_lock(&orderLock);
    while (currentOrder == nullptr)
    {
        pthread_cond_wait(&orderCond, &orderLock);
    }
    orderName = currentOrder->name;
    orderCnt = currentOrder->breadCnt;
    amountDone = 0;
    // std::cout << "Baking for: " << orderName << std::endl;
    pthread_cond_broadcast(&orderCond);
    pthread_mutex_unlock(&orderLock);
}
