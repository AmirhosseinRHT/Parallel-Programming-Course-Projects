#include <iostream>

class DeliverySpace
{
public:
    DeliverySpace(int cap);
    ~DeliverySpace();
    int addBakedBreads(pthread_mutex_t& DeliverySpaceLock, pthread_cond_t& DeliverySpaceCond , std::string name , int breadCnt);
    int pickupBakedBreads(pthread_mutex_t& DeliverySpaceLock,std::string name);


private:
    int capacity;
    int freeSpace;
    std::string * bakedBreads;
};