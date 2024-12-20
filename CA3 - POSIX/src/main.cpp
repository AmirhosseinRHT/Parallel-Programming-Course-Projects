#include "queue.cpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

void fillQueues(std::vector<Queue> &queues, std::string fileName)
{
    std::ifstream inputFile(fileName);
    std::cout << fileName << std::endl;
    if (!inputFile)
    {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }

    int n;
    inputFile >> n;
    inputFile.ignore();

    std::string namesLine;
    std::string breadsLine;
    for (int i = 0; i < n; i++)
    {
        Queue queue = Queue();

        std::getline(inputFile, namesLine);
        std::getline(inputFile, breadsLine);

        std::istringstream namesStream(namesLine);
        std::string name;
        std::vector<std::string> names;

        std::istringstream breadsStream(breadsLine);
        int breadCount;
        std::vector<int> breadCounts;

        while (namesStream >> name)
        {
            names.push_back(name);
        }

        while (breadsStream >> breadCount)
        {
            breadCounts.push_back(breadCount);
        }
        for (int j = 0; j < names.size(); j++)
        {
            Customer *customer = new Customer(names[j], breadCounts[j]);
            queue.addCustomer(customer);
        }
        queues.push_back(queue);
    }
    inputFile.close();
}

void *startQueue(void *arg)
{
    Queue *queue = static_cast<Queue *>(arg);
    pthread_t threadId = pthread_self();
    std::cout << "Thread ID: " << threadId << "\n";
    queue->printCustomers();
    // queue
}

int main(int argc, char *argv[])
{
    std::vector<Queue> queues;
    std::vector<pthread_t> queueThreads;
    std::vector<Customer *> currentCustomers;

    if (argc < 2)
    {
        std::cerr << "Pass the Input Pls." << std::endl;
        return 1;
    }
    fillQueues(queues, argv[1]);

    for (int i = 0; i < queues.size(); i++)
    {
        pthread_t thread;
        currentCustomers.push_back(nullptr);
        queues[i].setCurrentCustomer(currentCustomers[i]);
        if (pthread_create(&thread, nullptr, startQueue, &queues[i]) != 0)
        {
            std::cerr << "Failed to create thread " << i << "\n";
            return 1;
        }
        queueThreads.push_back(thread);
    }

    for (size_t i = 0; i < queueThreads.size(); ++i)
    {
        pthread_join(queueThreads[i], nullptr);
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
