
    #include <iostream>
    #include <chrono>
    #include <omp.h>

    const long long SIDE = 1000;
    using namespace std;


    uint64_t micros()
    {
        uint64_t us = chrono::duration_cast<chrono::microseconds>(
                chrono::high_resolution_clock::now().time_since_epoch())
                .count();
        return us; 
    }

    long calcPiSerial() {
        float x, y, origin_dist;
        int circle_points = 0;
        long long start , end;
        start = micros();
        for (int i = 0; i < (SIDE * SIDE); i++) {
            x = float(rand() % (SIDE)) / SIDE;
            y = float(rand() % (SIDE)) / SIDE;
            origin_dist = x * x + y * y;
            if (origin_dist < 1)
                circle_points++;
        }
        float pi = float(4 * circle_points) / (SIDE * SIDE);
        end = micros();
        cout << "Serial Result : " << pi << endl;
        return end - start;
    }

    long calcPiParallel() {
    float x, y, origin_dist;
    int circle_points = 0;
    long long start, end;
    start = micros();

    #pragma omp parallel loop private(circle_points) reduction(+:circle_points)
    for (int i = 0; i < (SIDE * SIDE); i++) {
        x = float(rand() % (SIDE)) / SIDE;
        y = float(rand() % (SIDE)) / SIDE;
        origin_dist = x * x + y * y;
        if (origin_dist < 1)
            circle_points++;
    }
    float pi = float(4 * circle_points) / (SIDE * SIDE);
    end = micros();
    cout << "Parallel Result : " << pi << endl;
    return end - start;
}

    int main()
    {
        srand(time(NULL));
        long serialTime = calcPiSerial();
        long parallelTime = calcPiParallel();
        cout << "Serial Time : " << serialTime << " microseconds" << endl;
        cout << "Parallel Time : " << parallelTime << " microseconds" << endl;
        float speedUp = float(serialTime  / parallelTime);
        cout << "SpeedUp: " << speedUp << endl;
        return 0;
    }
