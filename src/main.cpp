#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"
#include <filesystem>

#include "utils.h"
#include "compressor.h"
#include "decompressor.h"

using std::string;
using std::vector;

void print_usage_and_exit() {
    std::cout << "  Use this command-line tool to compress and decompress files.\n"          
              << "Usage: \n\n"
              << "  Compressing   -  fcmp compress <input_file_path>\n\n"
              << "  Decompressing -  fcmp decompress <input_file_path>\n\n"
              << "  \n"
              << "  The output file will have the same name as the input file but with a.fcm extension.\n\n"
              << "  The output file will be a binary file containing the compressed data.\n"
              << "  The compressed file can be decompressed using the decompress command.\n"
              << "  The decompressed file will have the same name as the input file but with the original extension.\n\n"
              << "\n"
              << "Examples: \n"
              << "  fcmp compress \"C:\\directory\\file.txt\" \n"
              << "      creates file_compressed.fcm in C:\\directory \n\n"
              << "  fcmp decompress \"C:\\directory\\file.txt\" \n"
              << "      creates original file in C:\\directory \n\n"

              << std::endl;
    exit(1);
}


int main(int argc, char *argv[]) {
    
    if (argc != 3) {
        print_usage_and_exit();
    }

    string input_file_path = argv[2];
    std::filesystem::path filePath(input_file_path);

    // We need to store the original file extension so we know what to decompress to 
    std::cout << "Input file path " << input_file_path << std::endl;
    
    string command = argv[1];
    if (command == "compress") {
        // Run compression program
        Compressor::HuffCompressor compressor;
        std::cout << "Compressing..... " << std::endl;
        vector<uint8_t> file_data = Utils::readFile(input_file_path);
        compressor.compress(input_file_path, file_data);

    } else if (command == "decompress") {
        
        std::cout << "Decompressing..... " << std::endl;
        Decompressor::HuffDecompressor decompressor;
        decompressor.decompress(input_file_path);

    } else {
        print_usage_and_exit();
    }

    return 0;
}