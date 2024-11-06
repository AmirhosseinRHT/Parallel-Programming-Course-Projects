#include <iostream>
#include <intrin.h>
#include <tmmintrin.h>
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
    // for (int i = 0; i < SAMPLE_SIZE; i++)
    // {
    //     sample_data[i] = 0.1;
    // }
    // sample_data[SAMPLE_SIZE-1] = 0.9;
    // return sample_data;

}

float calcAverageSerial(const float *sampleData)
{
    float sum = 0;
    for (int i = 0; i < SAMPLE_SIZE; i++)
        sum += sampleData[i];
    return sum / SAMPLE_SIZE;
}

float calcStandardDeviationSerial(const float *sampleData , float avg) {
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
    float standardDeviation = calcStandardDeviationSerial(sampleData, average);
    for (int i = 0; i < SAMPLE_SIZE; i++) {
        float z = (sampleData[i] - average) / standardDeviation;
        if(z > 2.5)
            count+=1;
    }
    end = micros();
    cout << "Serial Time: " << end - start << endl;
    return count;
}

__m128 calcAvgParallel(float* sampleData) {
    __m128 sum = _mm_setzero_ps();
    __m128 count = _mm_set1_ps(SAMPLE_SIZE);
    for (int i = 0; i < SAMPLE_SIZE; i += 4) {
        __m128 data = _mm_loadu_ps(&sampleData[i]);
        sum = _mm_add_ps(sum, data);
    }
    sum = _mm_hadd_ps(sum, sum);
    sum = _mm_hadd_ps(sum, sum);
    return _mm_div_ps(sum, count);
}

__m128 calcStandardDeviationParallel(float* sampleData, __m128 avg) {
    __m128 sum = _mm_setzero_ps();
    __m128 count = _mm_set1_ps(SAMPLE_SIZE);
    for (int i = 0; i < SAMPLE_SIZE; i += 4) {
        __m128 data = _mm_loadu_ps(&sampleData[i]);
        __m128 diff = _mm_sub_ps(data, avg);
        sum = _mm_add_ps(sum, _mm_mul_ps(diff, diff));
    }
    sum = _mm_hadd_ps(sum, sum);
    sum = _mm_hadd_ps(sum, sum);
    __m128 variance = _mm_div_ps(sum, count);
    __m128 standardDeviation = _mm_sqrt_ps(variance);
    return standardDeviation;
}

__m128i findOutliersParallel(float* sampleData) {
    long long start , end; 
    start = micros();

    __m128 avg = calcAvgParallel(sampleData);
    __m128 standardDeviation = calcStandardDeviationParallel(sampleData, avg);

    __m128i count = _mm_setzero_si128();
    for (int i = 0; i < SAMPLE_SIZE; i += 4) {
        __m128 data = _mm_loadu_ps(&sampleData[i]);
        __m128 diff = _mm_sub_ps(data, avg);
        __m128 z = _mm_div_ps(diff, standardDeviation);
        __m128 compareRes = _mm_cmpgt_ps(z, _mm_set1_ps(2.5));
        count = _mm_add_epi32(count, _mm_castps_si128(compareRes));
    }
    count = _mm_hadd_epi32(count, count);
    count = _mm_hadd_epi32(count, count);  
    end = micros();
    cout << "Parallel Time: " << end - start << endl;
    return count;
}

int main()
{
    // Serial
    float* sampleData = generateSampleData();
    int count = findOutliersSerial(sampleData);

    // Parallel
    __m128i count2 = _mm_abs_epi32(findOutliersParallel(sampleData));

    // Display results
    cout << "Serial Count: " << count << endl;
    cout << "Parallel Count: " << _mm_cvtsi128_si32(count2) << endl;

    //free
    free(sampleData);
    return 0;
}