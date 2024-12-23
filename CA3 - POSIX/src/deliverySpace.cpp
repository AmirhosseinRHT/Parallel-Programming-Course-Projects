#include "headers/deliverySpace.h"
#include <algorithm>

void DeliverySpace::pickupBakedBreads(pthread_mutex_t &deliverySpaceLock, std::string name)
{
    pthread_mutex_lock(&deliverySpaceLock);
    for (auto it = bakedBreads.begin(); it != bakedBreads.end();)
    {
        if (*it == name)
        {
            it = bakedBreads.erase(it);
        }
        else
        {
            ++it;
        }
    }
    pthread_mutex_unlock(&deliverySpaceLock);
}

void DeliverySpace::addBakedBreads(pthread_mutex_t &deliverySpaceLock, std::string name, int count)
{
    pthread_mutex_lock(&deliverySpaceLock);
    for (int i = 0; i < count; ++i)
    {
        bakedBreads.push_back(name);
    }
    pthread_mutex_unlock(&deliverySpaceLock);
}