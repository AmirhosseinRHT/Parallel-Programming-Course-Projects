#include<string>


class Customer{

public:
    Customer(std::string _name, int _breadCnt){
        name = _name;
        breadCnt = _breadCnt;
    }
    std::string getName(){
        return name;
    }
    int getBreadCnt(){
        return breadCnt;
    }
private:
    std::string name;
    int breadCnt;
};