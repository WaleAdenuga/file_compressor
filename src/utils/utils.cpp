#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdint>

#include "utils.h"

using std::string;
using std::vector;

namespace Utils {
    string vector2String(vector<uint8_t> data) {
        string str(data.begin(), data.end());
        return str;
    }
    
    vector<uint8_t> string2Vector(string data) {
        vector<uint8_t> result(data.data(), data.data() + data.size());
        return result;
    }

    vector<uint8_t> readFile(const string &filePath) {

        // output buffer
        vector<uint8_t> buffer;

        // open in binary mode - can be text, jpg or pdf (ate opens the file in binary mode and moves the pointer to the end of the file)
        std::ifstream file(filePath.c_str(), std::ios::binary | std::ios::ate);

        if (!file) {
            std::cerr << "Error opening file: " << filePath << std::endl;
            exit(1);
        }

        try {

            size_t fileSize = file.tellg();
            if (fileSize <= 0)
            {
                std::cerr << "Empty or invalid file size!" << std::endl;
                exit(1);
            }

            // resize buffer to fit the file
            buffer.resize(fileSize);
            // move file pointer to the beginning
            file.seekg(0, std::ios::beg);

            // read file into buffer
            if (!file.read(reinterpret_cast<char *>(buffer.data()), fileSize)) {
                std::cerr << "Error reading file data!" << std::endl;
                exit(1);
            }
            file.close();
        }
        catch (std::exception const &e) {
            std::cout << "Exception: " << e.what() << "\n";
        }
        return buffer;
    }

    bool writeFile(const string &filePath, vector<uint8_t> content) {
        std::ofstream file(filePath, std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file: " << filePath << std::endl;
            return false;
        }

        // write buffer to file
        try {
            if (!file.write(reinterpret_cast<const char *>(content.data()), content.size())) {
                std::cerr << "Error writing file data!" << std::endl;
                return false;
            }
            file.close();
            return true;
        }
        catch (std::exception const &e) {
            std::cout << "Exception: " << e.what() << "\n";
            return false;
        }
    }

    

}
