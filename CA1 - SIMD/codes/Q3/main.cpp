#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int ARRAY_SIZE = 50;
const unsigned int SEED = 10;

void generateRandomChars(char *arr, int size) {
    const char charset[] = "ABCDEF";
    // const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int charsetSize = sizeof(charset) - 1;
    for (int i = 0; i < size; i++) {
        arr[i] = charset[rand() % charsetSize];
    }
    arr[size] = '\0';
}

string runLengthEncodingSerial(const char *input) {
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
    return output;
}

int main() {
    srand(SEED);
    char *arr = new char[ARRAY_SIZE + 1];
    generateRandomChars(arr, ARRAY_SIZE);
    cout << "Input: " << arr << endl;
    string res = runLengthEncodingSerial(arr);
    cout << "Run Length Encoding: " << res << endl;
    delete[] arr;
    return 0;
}
