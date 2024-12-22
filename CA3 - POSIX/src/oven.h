
#ifndef OVEN_H
#define OVEN_H

#include <string>
#include <pthread.h>

class Oven
{
public:
    Oven(int cap);
    ~Oven();
    void addBreadToOven(std::string orderName,int orderCnt,int bakerId);
    bool updateFreeSpace(int change);
    std::string *getBakingBreads();
    int getFreeSpace();

    pthread_mutex_t lock;

private:
    int capacity;
    int freeSpace;
    std::string *bakingBreads;
};

#endif 