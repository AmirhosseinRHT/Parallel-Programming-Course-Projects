#include "queue.cpp"
#include <iostream>
#include <sstream>  
#include <fstream>  
#include <string>
#include <vector>



void fillQueues(std::vector<Queue>& queues,std::string fileName){
    std::ifstream inputFile(fileName);
    std::cout << fileName <<std::endl;
    if (!inputFile) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }

    int n;  
    inputFile >> n;  
    inputFile.ignore();


    std::string namesLine;  
    std::string breadsLine;
    for(int i =0; i < n; i++){
        std::cout << "hello?" << std::endl; 
        Queue queue = Queue();

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
        for(int j = 0; j < names.size(); j++){
            Customer* customer = new Customer(names[j],breadCounts[j]);
            queue.addCustomer(customer);
        }
        queues.push_back(queue);
    } 
    inputFile.close();
}






int main(int argc, char* argv[]){
    if (argc < 2) {
        std::cerr << "Pass the Input Pls." << std::endl;
        return 1;
    }
    std::vector<Queue> queues;
    fillQueues(queues,argv[1]);
    for(int i =0; i < queues.size(); i++){
        std::cout << "Queue: " << i << std::endl;
        std::vector<Customer*> customers = queues[i].getCustomers();
        for(int j=0; j < customers.size(); j++){
            std::cout << customers[j]->getName() <<std::endl;
            std::cout << customers[j]->getBreadCnt() <<std::endl;
        }
        
    }
    return 0;
}




