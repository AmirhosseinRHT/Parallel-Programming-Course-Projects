#include "utility.h"
#include <fstream>
#include <sstream>
#include <iostream>

void fillQueues(std::vector<Queue*>& queues, const std::string& fileName) {
    std::ifstream inputFile(fileName);
    if (!inputFile) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }

    int n;
    inputFile >> n;
    inputFile.ignore();

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
