
#ifndef OVEN_H
#define OVEN_H

#include <string>
#include <pthread.h>
#include <order.h>





class Oven
{
public:
    Oven(int cap);
    ~Oven();
    void addBreadToOven(std::string orderName,int orderCnt,int bakerId);
    bool updateFreeSpace(int change);
    std::string *getBakingBreads();
    int getFreeSpace();

private:
    int capacity;
    std::string* bakingBreads;
    long long* bakingTimer;
    int totalCap; 
    
};

#endif 