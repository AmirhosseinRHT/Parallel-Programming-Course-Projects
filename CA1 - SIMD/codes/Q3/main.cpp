#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

const int ARRAY_SIZE = 50;
const unsigned int SEED = 10;

uint64_t micros()
{
    uint64_t us = chrono::duration_cast<chrono::microseconds>(
            chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return us; 
}

void generateRandomChars(char *arr, int size) {
    const char charset[] = "ABCDEF";
    // const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int charsetSize = sizeof(charset) - 1;
    for (int i = 0; i < size; i++) {
        arr[i] = charset[rand() % charsetSize];
    }
    arr[size] = '\0';
}

long runLengthEncodingSerial(const char *input) {
    long long start, end;
    start = micros();
    string output;
    while (*input != '\0') {
        char currentChar = *input;
        int count = 1;
        while (*input == *(input + 1)) {
            count++;
            input++;
        }
        output += currentChar;
        output += to_string(count);
        input++;
    }
    cout << "Run Length Encoding: " << output << endl;
    end = micros();
    return end - start;
}

int main() {
    srand(SEED);
    char *arr = new char[ARRAY_SIZE + 1];
    generateRandomChars(arr, ARRAY_SIZE);
    cout << "Input: " << arr << endl;
    long serialTime = runLengthEncodingSerial(arr);
    cout << "Serial Time: " << serialTime << endl;
    delete[] arr;
    return 0;
}
