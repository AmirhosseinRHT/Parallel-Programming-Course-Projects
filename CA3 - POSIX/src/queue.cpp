
#include <vector>
#include <iostream>

#include "customer.cpp"

class Queue
{
private:
    std::vector<Customer *> customers;
    Customer *current;

public:
    void addCustomer(Customer *customer)
    {
        customers.push_back(customer);
    }
    void removeFromFirst()
    {
        if (!customers.empty())
        {
            customers.erase(customers.begin());
        }
    }
    Customer *getFirst()
    {
        return customers[0];
    }
    std::vector<Customer *> getCustomers()
    {
        return customers;
    }
    void runQueue(pthread_mutex_t &orderLock, pthread_cond_t &orderCond)
    {
        while ("I Want Bread😜")
        {
            pthread_mutex_lock(&orderLock);

            pthread_cond_wait(&orderCond, &orderLock);
        }
    }
    void setCurrentCustomer(Customer *customer)
    {
        current = customer;
    }
    void printCustomers()
    {
        for (int j = 0; j < customers.size(); j++)
        {
            std::cout << customers[j]->getName() << std::endl;
            std::cout << customers[j]->getBreadCnt() << std::endl;
        }
    }
};