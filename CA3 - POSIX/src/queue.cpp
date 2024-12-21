#include "queue.h"

void Queue::addCustomer(Customer* customer) {
    customers.push_back(customer);
}

void Queue::removeFirstCustomer() {
    if (!customers.empty()) {
        customers.erase(customers.begin());
    }
}

Customer* Queue::getFirstCustomer() {
    return customers.empty() ? nullptr : customers[0];
}

std::vector<Customer*> Queue::getCustomers() {
    return customers;
}

void Queue::printCustomers() {
    for (int j = 0; j < customers.size(); j++) {
        std::cout << customers[j]->getName() << std::endl;
        std::cout << customers[j]->getBreadCnt() << std::endl;
    }
}
