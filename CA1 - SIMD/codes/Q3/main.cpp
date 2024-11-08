#include <immintrin.h>
#include <iostream>
#include <chrono>
#include <intrin.h>
#include <vector>

using namespace std;

const int ARRAY_SIZE = 1024;

uint64_t micros()
{
    uint64_t us = chrono::duration_cast<chrono::microseconds>(
        chrono::high_resolution_clock::now().time_since_epoch())
        .count();
    return us;
}

void generateRandomChars(char *arr, int size) {
    const char charset[] = "ABCDEFGH";
    int numGroups = size / 16;
    int i = 0;
    for (int group = 0; group < numGroups; group++) {
        char randomChar = charset[rand() % 2];
        for (int j = 0; j < 16; j++) 
            arr[i++] = randomChar;
    }
    arr[size] = '\0';
}

long runLengthEncodingSerial(const char *input) {
    long long start, end;
    vector<string> output;
    start = micros();
    while (*input != '\0') {
        char currentChar = *input;
        int count = 1;
        while (*input == *(input + 1)) {
            count++;
            input++;
        }
        output.push_back(string(1, currentChar));
        output.push_back(to_string(count));
        input++;
    }
    end = micros();
    cout << "Serial Result: ";
    for(int i = 0; i < output.size(); i++) 
        cout << output[i];
    cout << endl;
    return end - start;
}

int countConsecutiveOnes(__m128i value) {
    __m128i oneMask = _mm_set1_epi8(0x01);
    __m128i cmpResult = _mm_cmpeq_epi8(value, oneMask);
    int mask = _mm_movemask_epi8(cmpResult);
    return __builtin_ctz(~mask);
}

long runLengthEncodingParallel(const char *input) {
    long long start, end;
    int charCount = 0;
    vector<string> output;
    start = micros();
    for (int i = 0; i < ARRAY_SIZE; i++) {
        char current = input[i];
        __m128i extendedChar = _mm_set1_epi8(current);
        __m128i nextLoaded = _mm_loadu_si128((__m128i*)(input + i + 1));
        __m128i compareResult = _mm_cmpeq_epi8(extendedChar, nextLoaded);
        compareResult = _mm_and_si128(compareResult, _mm_set1_epi8(1));
        int totalConsecutiveOnes = countConsecutiveOnes(compareResult);
        
        //comparison with 16
        __m128i totalConsecutiveOnesRegister = _mm_set1_epi32(totalConsecutiveOnes);
        __m128i sixteen = _mm_set1_epi32(16);
        __m128i cmpResult = _mm_cmpeq_epi32(totalConsecutiveOnesRegister, sixteen);
        int mask = _mm_movemask_ps(_mm_castsi128_ps(cmpResult));
        
        //addition
        __m128i charCountRegister = _mm_set1_epi32(charCount);
        __m128i one = _mm_set1_epi32(1);
        __m128i sumConsecutiveOnes = _mm_add_epi32(_mm_add_epi32(charCountRegister, totalConsecutiveOnesRegister), one);
        
        if (mask == 0) { 
            output.push_back(string(1, current));
            int sum;
            _mm_store_ss((float*)&sum, _mm_castsi128_ps(sumConsecutiveOnes));
            output.push_back(to_string(sum));
            charCount = 0;
        } else 
            _mm_store_ss((float*)&charCount, _mm_castsi128_ps(sumConsecutiveOnes));
        
        i += totalConsecutiveOnes;
    }
    end = micros();
    cout << "Parallel Result: ";
    for (int i = 0; i < output.size(); i++)
        cout << output[i];
    cout << endl;
    return end - start;
}

int main() {
    srand(time(nullptr));
    char *arr = new char[ARRAY_SIZE + 1];
    generateRandomChars(arr, ARRAY_SIZE);
    cout << "Input: " << arr << endl;

    long serialTime = runLengthEncodingSerial(arr);
    cout << "Serial Time: " << serialTime << endl;

    long parallelTime = runLengthEncodingParallel(arr);
    cout << "Parallel Time: " << parallelTime << endl;

    cout << "Speed Up: " << (double) serialTime / (double) parallelTime << endl;

    delete[] arr;
    return 0;
}
