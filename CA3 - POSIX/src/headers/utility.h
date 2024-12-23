#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
#include <chrono>
#include "queue.h"

void fillQueues(std::vector<Queue *> &queues, const std::string &fileName, std::string modew);
int fillQueuesChaos(std::vector<Customer *> &customers, const std::string &fileName, std::string mode);
long long getCurrentTime();

#endif