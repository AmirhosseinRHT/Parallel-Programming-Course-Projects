#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <intrin.h>
#include <vector>

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
    const char charset[] = "AB";
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

long runLengthEncodingParallel(const char *input) {
    long long start, end;
    start = micros();
    vector<string> output;
    /////////////////////////////////////
    for (int i = 0; i < ARRAY_SIZE; i++) {
        char current = input[i];
        output.push_back(string(1 , current));
        __m128i extendedChar = _mm_set1_epi8(current);
        __m128i nextLoaded = _mm_loadu_si128((__m128i*)(input + i + 1));
        __m128i equal = _mm_cmpeq_epi8(extendedChar, nextLoaded);
        equal = _mm_and_si128(equal, _mm_set1_epi8(1));
        __m128i sum = _mm_sad_epu8(equal, _mm_setzero_si128());
        int total = _mm_extract_epi16(sum, 0) + _mm_extract_epi16(sum, 4);
        output.push_back(std::to_string(total));
        i += _mm_extract_epi16(sum, 0) + _mm_extract_epi16(sum, 4);
    }
    for (int i = 0; i < output.size(); i++) {
        cout << output[i];
    }
    ////////////////////////////////////
    end = micros();
    return end - start;
}

int main() {
    srand(SEED);
    char *arr = new char[ARRAY_SIZE + 1];
    generateRandomChars(arr, ARRAY_SIZE);
    // cout << "Input: " << arr << endl;
    // long serialTime = runLengthEncodingSerial(arr);
    // cout << "Serial Time: " << serialTime << endl;
    runLengthEncodingParallel(arr);
    delete[] arr;
    return 0;
}
