#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include <iostream>
#include "customer.h"

class Queue
{
private:
    std::vector<Customer *> customers;

public:
    void addCustomer(Customer *customer);
    void removeFirstCustomer();
    Customer *getFirstCustomer();
    std::vector<Customer *> getCustomers();
    void printCustomers();
    int getQueueSize();
};

#endif
