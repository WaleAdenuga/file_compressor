#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"

#include "utils.h"
#include "compressor.h"

using std::string;
using std::vector;

void print_usage_and_exit() {
    std::cout << "  Use this tool to compress file sizes.\n" 
              << "Usage: fcmp <input_file> [output_file]\n"
              << "\n"
              << "  Providing a name for output_file is optional\n"
              << "\n"
              << "Example: \n"
              << "  fcmp C:\\directory\\file.txt \n"
              << "      creates file_compressed.txt in C:\\directory \n"
              << "  fcmp C:\\directory\\file.txt new_file \n"
              << "      creates new_file.txt in C:\\directory \n"
              << std::endl;
    exit(1);
}


int main(int argc, char *argv[]) {
    
    if (argc < 2 || argc > 3) {
        print_usage_and_exit();
    }

    string input_file_path = argv[1];
    // file name split at last dot ie test.txt returns test
    size_t position = input_file_path.find_last_of(".");
    string output_file_path = argc == 3? argv[2] : input_file_path.substr(0, position) + "_compressed.txt";


    std::cout << "Input file path " << input_file_path << std::endl;

    vector<uint8_t> file_data = Utils::readFile(input_file_path);

    // Run compression program
    Compressor::HuffCompressor compressor;
    compressor.compress(file_data, output_file_path);

    std::cout << "Compressed file saved to: " << output_file_path << std::endl;

    return 0;
}