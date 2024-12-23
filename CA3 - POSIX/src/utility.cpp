#include "utility.h"
#include <fstream>
#include <sstream>
#include <iostream>

void fillQueues(std::vector<Queue*>& queues, const std::string& fileName,std::string mode) {
    std::ifstream inputFile(fileName);
    if (!inputFile) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }
    int n;
    if (mode == "multi"){
        inputFile >> n;
        inputFile.ignore();
    }else{
        n = 1;
    }
    std::string namesLine, breadsLine;
    for (int i = 0; i < n; i++) {
        Queue* queue = new Queue();
        std::getline(inputFile, namesLine);
        std::getline(inputFile, breadsLine);

        std::istringstream namesStream(namesLine);
        std::string name;
        std::vector<std::string> names;

        std::istringstream breadsStream(breadsLine);
        int breadCount;
        std::vector<int> breadCounts;

        while (namesStream >> name) {
            names.push_back(name);
        }

        while (breadsStream >> breadCount) {
            breadCounts.push_back(breadCount);
        }

        for (int j = 0; j < names.size(); j++) {
            Customer* customer = new Customer(names[j], breadCounts[j]);
            queue->addCustomer(customer);
        }
        queues.push_back(queue);
    }

    inputFile.close();
}





int fillQueuesChaos(std::vector<Customer*>& customers, const std::string& fileName,std::string mode) {
    std::ifstream inputFile(fileName);
    if (!inputFile) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return 0;
    }
    int n;
    if (mode == "multi"){
        inputFile >> n;
        inputFile.ignore();
    }else{
        n = 1;
    }
    std::string namesLine, breadsLine;
    for (int i = 0; i < n; i++) {
        std::getline(inputFile, namesLine);
        std::getline(inputFile, breadsLine);

        std::istringstream namesStream(namesLine);
        std::string name;
        std::vector<std::string> names;

        std::istringstream breadsStream(breadsLine);
        int breadCount;
        std::vector<int> breadCounts;

        while (namesStream >> name) {
            names.push_back(name);
        }

        while (breadsStream >> breadCount) {
            breadCounts.push_back(breadCount);
        }

        for (int j = 0; j < names.size(); j++) {
            Customer* customer = new Customer(names[j], breadCounts[j]);
            customer->setQueueId(i);
            customers.push_back(customer);
        }
    }
    inputFile.close();
    return n;
}





long long getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}