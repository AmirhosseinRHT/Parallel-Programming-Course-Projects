#ifndef DELIVERYSPACE_H
#define DELIVERYSPACE_H

#include <vector>
#include <string>
#include <pthread.h>

class DeliverySpace {
public:
    void pickupBakedBreads(pthread_mutex_t& deliverySpaceLock, std::string name);
    void addBakedBreads(pthread_mutex_t& deliverySpaceLock, std::string name, int count);
private:
    std::vector<std::string> bakedBreads;
};


#endif