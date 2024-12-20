#include <iostream>
#include "customer.cpp"
#include "baker.cpp"

class Order
{
public:
    Order(Baker *_baker, Customer *_customer, int _count)
    {
        customer = _customer;
        baker = _baker;
        count = _count;
    }
    Baker *getBaker() { return baker; }
    Customer *getCustomer() { return customer; }
    int getCount() { return count; }

private:
    Customer *customer;
    Baker *baker;
    int count;
};
