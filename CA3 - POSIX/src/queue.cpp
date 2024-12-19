
#include<vector>
#include<iostream>

#include "customer.cpp"



class Queue{
private:
    std::vector<Customer> customers;

public:
    void addCustomer(Customer customer){
        customers.push_back(customer);
    }
    void removeFromFirst(){
        if (!customers.empty()) {
            customers.erase(customers.begin());
        }
    }
    Customer get_first(){
        return customers[0];
    }
};