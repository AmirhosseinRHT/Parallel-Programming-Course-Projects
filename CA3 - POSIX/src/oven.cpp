#include <iostream>
#include <vector>

class Oven
{

public:
    Oven(int cap)
    {
        capacity = cap;
        bakingBreads = new std::string[capacity];
        freeSpace = capacity;
        for (int i = 0; i < capacity; i++)
        {
            bakingBreads[i] = "";
        }
    }

    ~Oven()
    {
        delete[] bakingBreads;
    }

    bool updateFreeSpace(int change)
    {
        if (0 <= freeSpace + change <= capacity)
        {
            freeSpace += change;
            return true;
        }
        return false;
    }

    std::string *getBakingBreads() { return bakingBreads; }
    int getFreeSpace() { return freeSpace; }
    pthread_mutex_t lock;

private:
    int capacity;
    int freeSpace;
    std::string *bakingBreads;
};