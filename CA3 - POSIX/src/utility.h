#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
#include <chrono>
#include "queue.h"

void fillQueues(std::vector<Queue*>& queues, const std::string& fileName);
long long getCurrentTime();

#endif 