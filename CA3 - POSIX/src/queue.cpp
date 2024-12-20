
#include<vector>
#include<iostream>

#include "customer.cpp"



class Queue{
private:
    std::vector<Customer*> customers;

public:
    void addCustomer(Customer* customer){
        customers.push_back(customer);
    }
    void removeFromFirst(){
        if (!customers.empty()) {
            customers.erase(customers.begin());
        }
    }
    Customer* getFirst(){
        return customers[0];
    }
    std::vector<Customer*> getCustomers(){
        return customers;
    }
};