#include <iostream>
#include "deliverySpace.h"


DeliverySpace::DeliverySpace(int cap){
    capacity = cap;
    freeSpace = cap;
    bakedBreads = new std::string[capacity];
}


DeliverySpace::~DeliverySpace(){
    delete[] bakedBreads;
}


int DeliverySpace::pickupBakedBreads(pthread_mutex_t& DeliverySpaceLock, std::string name) {
    pthread_mutex_lock(&DeliverySpaceLock);
        int pickedUp = 0;
    for(int i = 0; i < capacity; i++){    
        if(bakedBreads[i] == name){
            bakedBreads[i] = "";
            freeSpace++;
            pickedUp++;
        }
    }
    return pickedUp;
    pthread_mutex_unlock(&DeliverySpaceLock);
    return pickedUp;
}

int DeliverySpace::addBakedBreads(pthread_mutex_t& DeliverySpaceLock,
                         pthread_cond_t& DeliverySpaceCond ,std::string name , int breadCnt){
    pthread_mutex_lock(&DeliverySpaceLock);
    while(freeSpace < breadCnt){
        pthread_cond_wait(&DeliverySpaceCond, &DeliverySpaceLock);
    }
    for (int i = 0; i < breadCnt; i++){
        bakedBreads[i] = name;
        freeSpace--;
    }
    pthread_cond_broadcast(&DeliverySpaceCond);
    pthread_mutex_unlock(&DeliverySpaceLock);
    return freeSpace;
}