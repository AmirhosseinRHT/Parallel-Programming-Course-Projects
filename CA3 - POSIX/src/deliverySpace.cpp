#include <iostream>

class DeliverySpace
{
public:
DeliverySpace(){
    
}

private:


int capacity;
int freeSpace;
pthread_mutex_t lock;
std::string * bakedBreads;

};