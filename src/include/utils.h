#pragma once // only include this file once, regardless of how many times you come across it
#include <string>

#include <iostream>
#include <vector>
#include <cstdint>

using std::string;
using std::vector;

namespace Utils {
    string vector2String(vector<uint8_t> data);
    vector<uint8_t> string2Vector(string data);
    vector<uint8_t> readFile(const string &filePath);
    bool writeFile(const string &filePath, vector<uint8_t> content);
    
    
    // Append a value to the buffer as a series of bytes. Works for any type that can be converted to a byte array.
    // Has ti be here - templates are compile-time constructs and the compiler only generates their code when it sees the actual ype being used
    // each .cpp file that includes this header can instantiate the template function with the specific types it uses
    template <typename T>
    void appendToBuffer(std::vector<uint8_t>& buffer, T value) {
        uint8_t* dataPtr = reinterpret_cast<uint8_t*>(&value);
        buffer.insert(buffer.end(), dataPtr, dataPtr + sizeof(T));
    }
}