#include<string>


class Customer{

public:
    Customer(std::string _name, int _bread_cnt){
        name = _name;
        bread_cnt = _bread_cnt;
    }
    
private:
    std::string name;
    int bread_cnt;
};