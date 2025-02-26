#include "headers/oven.h"
#include "headers/utility.h"
#include <iostream>

void Oven::addBreadToOven(std::string orderName, int orderCnt)
{
    // this is critical section and should be locked from outside
    int c = 0;
    long long currentTime = getCurrentTime();
    for (int i = 0; i < totalCap; i++)
    {
        if (c > orderCnt)
        {
            break;
        }
        if (bakingBreads[i] == "")
        {
            bakingBreads[i] = orderName;
            bakingTimer[i] = currentTime;
            c++;
        }
    }
    capacity -= orderCnt;
}

void Oven::recieveBread(std::string orderName)
{
    // this is critical section and should be locked from outside
    for (int i = 0; i < totalCap; i++)
    {
        if (bakingBreads[i] == orderName)
        {
            bakingBreads[i] = "";
            bakingTimer[i] = 0;
            capacity++;
        }
    }
}

Oven::Oven(int cap)
{
    capacity = cap;
    totalCap = cap;
    bakingBreads = new std::string[capacity];
    bakingTimer = new long long[capacity];
    for (int i = 0; i < capacity; i++)
    {
        bakingBreads[i] = "";
        bakingTimer[i] = 0;
    }
}

Oven::~Oven()
{
    delete bakingBreads;
    delete bakingTimer;
}

int Oven::getFreeSpace()
{
    return capacity;
}
