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
pthread_mutex_t deliverySpaceLock;






void *runQueue(void *arg) {
    int queue_id = (intptr_t)arg;
    Queue* queue = queues[queue_id];
    while(queue->getQueueSize() != 0){
        Customer* firstCustomer = queue->getFirstCustomer();
        queue->removeFirstCustomer();
        std::cout << "\033[1;34m[Customer Log]\033[0m \033[1;32mFirst Customer: \033[0m" << firstCustomer->getName() << " \033[1;34mAnnouncing at: \033[0m" << "\033[1;33m" << getCurrentTime() << "\033[0m" << std::endl;
        long long start = getCurrentTime();
        firstCustomer->announceOrder(orderLocks[queue_id], orderConds[queue_id], currentOrders[queue_id]);
        firstCustomer->waitForBread(orderLocks[queue_id], orderConds[queue_id], currentOrders[queue_id]);
        std::cout << "\033[1;34m[Customer Log]\033[0m \033[1;32mThe Order for: \033[0m" << firstCustomer->getName() << " \033[1;34mis Ready at: \033[0m" << "\033[1;33m" << getCurrentTime() << "\033[0m" << std::endl;
        deliverySpace->pickupBakedBreads(deliverySpaceLock , firstCustomer->getName());
        std::cout << "\033[1;34m[Customer Log]\033[0m \033[1;32mThe Customer with name \033[0m" << firstCustomer->getName() << " \033[1;34mReceived the Order at: \033[0m" << "\033[1;33m" << getCurrentTime() << "\033[0m" << std::endl;
        std::cout << "\033[1;31m[Customer Log]\033[0m \033[1;31mFrom Ordering To Recievieng breads: \033[0m" << getCurrentTime() - start << std::endl;
        free(currentOrders[queue_id]);
        currentOrders[queue_id] = nullptr;

    }

}

void *runBaker(void *arg){
    int baker_id = (intptr_t)arg;
    Baker* baker = bakers[baker_id];
    while("Baking Bread 😋"){
        std::cout << "\033[1;35m[Baker Log]\033[0m Setuped the Current Order" << std::endl;
        baker->waitForOrder(orderLocks[baker_id], orderConds[baker_id], currentOrders[baker_id]);
        long long start = getCurrentTime();
        std::cout << "\033[1;35m[Baker Log]\033[0m \033[1;34mOrder Received\033[0m" << std::endl;
        baker->BakeOrder(ovenLock, ovenCond, oven);
        std::cout << "\033[1;35m[Baker Log]\033[0m \033[1;34mOrders Baked\033[0m " << currentOrders[baker_id]->name << " \033[1;34mis Ready at: \033[0m" << "\033[1;33m" << getCurrentTime() << "\033[0m" << std::endl;
        deliverySpace->addBakedBreads(deliverySpaceLock, currentOrders[baker_id]->name, currentOrders[baker_id]->breadCnt);
        std::cout << "\033[1;35m[Baker Log]\033[0m \033[1;34mOrders Added to Delivery Space\033[0m" << std::endl;
        pthread_mutex_lock(&orderLocks[baker_id]);
        currentOrders[baker_id] = nullptr;
        pthread_cond_broadcast(&orderConds[baker_id]);
        pthread_mutex_unlock(&orderLocks[baker_id]);
        std::cout << "\033[1;31m[Baker Log]\033[0m \033[1;31mFrom Order Arriving To Delivery: \033[0m" << getCurrentTime() - start << std::endl;
        // std::cout << "Order Done" << std::endl;
    }
}









int main(int argc, char* argv[]){
    std::cout << "The Second Input of the Program Should be Either \'multi\' or \'single\'" << std::endl;
    std::vector<pthread_t> queueThreads; 
    std::vector<pthread_t> bakerThreads;
    deliverySpace = new DeliverySpace();
    pthread_mutex_init(&ovenLock, NULL);
    pthread_cond_init(&ovenCond, NULL);
    pthread_mutex_init(&deliverySpaceLock, NULL);

    if (argc < 2) {
        std::cerr << "Pass the Input Pls." << std::endl;
        return 1;
    }
    fillQueues(queues,argv[1],argv[2]);
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



    for (size_t i = 0; i < queueThreads.size(); i++) {
        pthread_join(queueThreads[i], nullptr);
    }
    
    for (size_t i = 0; i < bakerThreads.size(); i++) {  
        pthread_cancel(bakerThreads[i]);  
    }  

    for (size_t i = 0; i < bakerThreads.size(); i++) {  
        pthread_join(bakerThreads[i], nullptr);  
    }  

    for (size_t i = 0; i < orderLocks.size(); i++) {
        pthread_mutex_destroy(&orderLocks[i]);
        pthread_cond_destroy(&orderConds[i]);
    }

    pthread_mutex_destroy(&ovenLock);
    pthread_cond_destroy(&ovenCond);
    pthread_mutex_destroy(&deliverySpaceLock);

    return 0;
}
