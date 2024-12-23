#include "utility.h"
#include "baker.h"
#include "oven.h"
#include "deliverySpace.h"
#include <iostream>
#include <sstream>  
#include <fstream>  
#include <unistd.h>
#include <string>
#include <vector>



std::vector<Queue*> queues;
std::vector<Baker*> bakers;
std::vector<pthread_mutex_t> orderLocks;
std::vector<pthread_cond_t> orderConds;
std::vector<struct Order*> currentOrders;
Oven* oven;
DeliverySpace* deliverySpace;
pthread_mutex_t ovenLock;
pthread_cond_t ovenCond;
pthread_cond_t orderCond;
pthread_mutex_t deliverySpaceLock;
pthread_cond_t deliverySpaceCond;






void *runQueue(void *arg) {
    int queue_id = (intptr_t)arg;
    Queue* queue = queues[queue_id];
    Customer* firstCustomer = queue->getFirstCustomer();
    std::cout << "First Customer: " << firstCustomer->getName() << std::endl;
    firstCustomer->announceOrder(orderLocks[queue_id], orderConds[queue_id], currentOrders[queue_id]);
    std::cout << "Successfully Announced" << std::endl;
    firstCustomer->waitForBread(orderLocks[queue_id], orderConds[queue_id], currentOrders[queue_id]);
    std::cout << "Pick Up Time" <<std::endl;
    //implement sharedspacePickup
    // queue->printCustomers();
    deliverySpace->pickupBakedBreads(deliverySpaceLock , firstCustomer->getName());
}

void *runBaker(void *arg){
    int baker_id = (intptr_t)arg;
    Baker* baker = bakers[baker_id];
    std::cout << "Setuped the Current Order" << std::endl;
    baker->waitForOrder(orderLocks[baker_id],orderConds[baker_id],currentOrders[baker_id]);
    std::cout << "Order Recieved" << std::endl;
    ///////bake order

    deliverySpace->addBakedBreads(deliverySpaceLock, deliverySpaceCond, currentOrders[baker_id]->name, currentOrders[baker_id]->breadCnt);
}


// void *runOvenTimer(void *arg){
//     while("Timing"){
//         sleep(2);
//         oven->updateOven();
//     }
// }







int main(int argc, char* argv[]){
    std::vector<pthread_t> queueThreads; 
    std::vector<pthread_t> bakerThreads;
    oven = new Oven(100);
    deliverySpace = new DeliverySpace(100);
    pthread_mutex_init(&ovenLock, NULL);
    pthread_cond_init(&ovenCond, NULL);
    pthread_cond_init(&orderCond, NULL);
    pthread_mutex_init(&deliverySpaceLock, NULL);
    pthread_cond_init(&deliverySpaceCond, NULL);

    if (argc < 2) {
        std::cerr << "Pass the Input Pls." << std::endl;
        return 1;
    }
    fillQueues(queues,argv[1]);

    oven = new Oven(queues.size()*10);
    std::cout << "Filled Queues" <<std::endl;
    for(int i =0; i< queues.size();i++){
        pthread_mutex_t mutex;
        pthread_cond_t cond;
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&cond, NULL);
        currentOrders.push_back(nullptr);
        orderLocks.push_back(mutex);
        orderConds.push_back(cond);
        Baker* baker = new Baker();
        bakers.push_back(baker);
    }

    for(int i =0; i < queues.size(); i++){
        pthread_t thread;
        if (pthread_create(&thread, nullptr, runBaker, (void *)(intptr_t)i) != 0) {
            std::cerr << "Failed to create thread " << i << "\n";
            return 1;
        }
        bakerThreads.push_back(thread);
    }

    for(int i = 0; i < queues.size();i++){
        pthread_t thread;
        if (pthread_create(&thread, nullptr, runQueue, (void *)(intptr_t)i) != 0) {
            std::cerr << "Failed to create thread " << i << "\n";
            return 1;
        }
        queueThreads.push_back(thread);

    }



    for (size_t i = 0; i < queueThreads.size(); ++i) {
        pthread_join(queueThreads[i], nullptr);
        pthread_join(bakerThreads[i], nullptr);
    }

    // for(int i =0; i < queues.size(); i++){
    //     std::cout << "Queue: " << i << std::endl;
    //     std::vector<Customer*> customers = queues[i].getCustomers();
    //     for(int j=0; j < customers.size(); j++){
    //         std::cout << customers[j]->getName() <<std::endl;
    //         std::cout << customers[j]->getBreadCnt() <<std::endl;
    //     }
        
    // }
    return 0;
}




