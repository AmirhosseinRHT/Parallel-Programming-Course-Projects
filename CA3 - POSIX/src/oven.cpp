#include "oven.h"
#include "utility.h"
#include <iostream>




void Oven::addBreadToOven(std::string orderName,int orderCnt,int bakerId){
    
}

Oven::Oven(int cap)
{
    capacity = cap;
    bakingBreads = new std::string[capacity];
    freeSpace = capacity;
    for (int i = 0; i < capacity; i++)
    {
        bakingBreads[i] = "";
    }
}

Oven::~Oven()
{
    delete[] bakingBreads;
}

bool Oven::updateFreeSpace(int change)
{
    if (0 <= freeSpace + change && freeSpace + change <= capacity)
    {
        freeSpace += change;
        return true;
    }
    return false;
}

std::string *Oven::getBakingBreads()
{
    return bakingBreads;
}

int Oven::getFreeSpace()
{
    return freeSpace;
}
