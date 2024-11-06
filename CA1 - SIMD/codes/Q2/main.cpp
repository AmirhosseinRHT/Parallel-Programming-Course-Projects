#include <iostream>
#include <intrin.h>
#include <random>
#include <cmath>
#include <chrono>

using namespace std;

const long SAMPLE_SIZE = 1024 * 1024;

uint64_t micros()
{
    uint64_t us = chrono::duration_cast<chrono::microseconds>(
            chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return us; 
}

float* generateSampleData() {
    auto* sample_data = new float[SAMPLE_SIZE];
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dist(0.0, 1.0);

    for (int i = 0; i < SAMPLE_SIZE; i++) 
        sample_data[i] = dist(gen);
    return sample_data;
}

float calcAverageSerial(const float *sampleData)
{
    float sum = 0;
    for (int i = 0; i < SAMPLE_SIZE; i++)
        sum += sampleData[i];
    return sum / SAMPLE_SIZE;
}

float calcStandardDeviation(const float *sampleData , float avg) {
    float squaredDiffSum = 0;

    for (int i = 0; i < SAMPLE_SIZE; i++)
        squaredDiffSum += (sampleData[i] - avg) * (sampleData[i] - avg);
    return sqrt(squaredDiffSum / SAMPLE_SIZE);
}

int findOutliersSerial(float* sampleData) {
    long long start , end; 
    start = micros();
    int count = 0;
    float average = calcAverageSerial(sampleData);
    float standardDeviation = calcStandardDeviation(sampleData, average);
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        float z = (sampleData[i] - average) / standardDeviation;
        if(z > 2.5)
            count+=1;
    }
    end = micros();
    cout << "Serial Time: " << end - start << endl;
    return count;
}

int main()
{
    float* sampleData = generateSampleData();
    int count = findOutliersSerial(sampleData);
    free(sampleData);
    return 0;
}