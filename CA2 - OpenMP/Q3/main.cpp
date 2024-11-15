
#include <iostream>
#include <chrono>
#include <omp.h>

const long long SIDE = 20000;
using namespace std;


uint64_t micros(){
    uint64_t us = chrono::duration_cast<chrono::microseconds>(
            chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return us; 
}

long long calcPiSerial() {
    float x, y, distanceToOrigin;
    int pointsInCircleCount = 0;
    long long start , end;
    start = micros();
    for (int i = 0; i < (SIDE * SIDE); i++) {
        x = float(rand() % (SIDE)) / SIDE;
        y = float(rand() % (SIDE)) / SIDE;
        distanceToOrigin = x * x + y * y;
        if (distanceToOrigin < 1)
            pointsInCircleCount++;
    }
    float pi = float(4 * pointsInCircleCount) / (SIDE * SIDE);
    end = micros();
    cout << "Serial Result : " << pi << endl;
    return end - start;
}

long long calcPiParallel() {
    double x, y, distanceToOrigin;
    int pointsInCircleCount = 0;
    long long start, end;

    start = micros();
    #pragma omp parallel for private(x, y, distanceToOrigin) reduction(+:pointsInCircleCount)
    for (int i = 0; i < (SIDE * SIDE); i++) {
        x = double(rand() % (SIDE)) / SIDE;
        y = double(rand() % (SIDE)) / SIDE;
        distanceToOrigin = x * x + y * y;
        if (distanceToOrigin < 1)
            pointsInCircleCount++;
    }

    double pi = double(4 * pointsInCircleCount) / (SIDE * SIDE);
    end = micros();

    cout << "Parallel Result : " << pi << endl;
    return end - start;
}

int main(){
    srand(time(NULL));
    long long serialTime = calcPiSerial();
    long long parallelTime = calcPiParallel();
    cout << "Serial Time : " << serialTime << " microseconds" << endl;
    cout << "Parallel Time : " << parallelTime << " microseconds" << endl;
    double speedUp = double (serialTime) / double (parallelTime);
    cout << "Speed Up: " << speedUp << endl;
    return 0;
}
