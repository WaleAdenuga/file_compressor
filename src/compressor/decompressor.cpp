#include "decompressor.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace Decompressor {
    
    void HuffDecompressor::decompress (const std::filesystem::path& inputFilePath) {
        // Read the data from the compressed file
        // Layout of input file looks like this:
        /**
         * 
         *  +-------------------------+
            | nameSize (uint32_t)     |  // e.g., 5
            +-------------------------+
            | origfileName (uint16_t) |  // e.g., photo.jpg, test.txt
            +-------------------------+
            | tableSize (uint32_t)    |  // e.g., 6
            +-------------------------+
            | byte  | frequency       |  // 'a', 5
            | byte  | frequency       |  // 'b', 9
            | ...                     |
            +-------------------------+
            | totalBits (uint32_t)    |  // Number of bits in compressed data (e.g., 12)
            +-------------------------+
            | compressed data bytes   |  // 0xD7, 0x20, etc.
            +-------------------------+
         */
        std::ifstream inputFile(inputFilePath, std::ios::binary);
        if(!inputFile.is_open()) {
            std::cerr << "Error opening compressed file at: " << inputFilePath << std::endl;
            exit(1);
        }

        // Read original file name size and file name
        uint32_t fileNameSize = 0;
        // read takes a character array and the maximum size of characters to store in said array
        inputFile.read(reinterpret_cast<char*>(&fileNameSize), sizeof(fileNameSize));

        string originalFileName(fileNameSize, '\0');
        inputFile.read(reinterpret_cast<char*>(&originalFileName[0]), fileNameSize);

        // Read frequency table size
        uint32_t tableSize = 0;
        inputFile.read(reinterpret_cast<char*>(&tableSize), sizeof(tableSize));

        // Read frequency table
        for (uint32_t i = 0; i<tableSize; i++) {
            uint8_t byte = 0;
            int frequency = 0;

            inputFile.read(reinterpret_cast<char*>(&byte), sizeof(byte));
            inputFile.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));

            frequencyTable[byte] = frequency;
        }

        // Read total bits
        uint32_t totalBits = 0;
        inputFile.read(reinterpret_cast<char*>(&totalBits), sizeof(totalBits));

        // Read compressed data (rest of file)
        size_t size = (totalBits + 7) / 8; // calculate bytes to store all bits
        vector<uint8_t> compressedData(size);
        inputFile.read(reinterpret_cast<char*>(compressedData.data()), size); 

        // Create huffman tree from frequencytable
        Compressor::HuffCompressor compressor;
        root = compressor.buildHuffmanTree(frequencyTable);

        // Decode the compressed data
        vector<uint8_t> decodedData;
        if (root) {
            decodedData = decodeCompressedData(totalBits, compressedData);
            
        } else {
            std::cerr << "Error decompressing file during decode." << std::endl;
            exit(1);
        }

        // Write the decoded data to an output file with the original file name
        if (!decodedData.empty()) {
            writeDecodedData(originalFileName, decodedData);
        } else {
            std::cerr << "Error decompressing file during write." << std::endl;
            exit(1);
        } 
        inputFile.close();
    }

    /// @brief Decode compressed data. 
    /// Start at the root, read bits, move left if 0 (right if 1), 
    /// When we reach a leaf node, extract data and append to output, then reset to root and continue decoding
    /// @param totalBits size of compressed data in bits
    /// @param compressedData actual compressed data
    /// @return array of decoded data
    vector<uint8_t> HuffDecompressor::decodeCompressedData(const uint32_t &totalBits, const vector<uint8_t> &compressedData) {
        vector<uint8_t> decodedData;
        if (!root) {
            throw std::runtime_error("Huffman tree not initialized!");
        }
        HuffmanNode* currentNode = root;

        int bitCount = 0;
        for (uint8_t byte : compressedData) {
            for (int i = 7; i >= 0; --i) {
                if (bitCount >= totalBits) { // stop processing if all bits are read - avoid padding buts
                    return decodedData;
                }
                bool bit = (byte >> i) & 1;

                currentNode = bit ? currentNode->right : currentNode->left;

                // If we hit a leaf node, output the symbol
                // We hit a leaf node if the left and right are null
                if (!currentNode->left && !currentNode->right) {
                    decodedData.push_back(currentNode->data);
                    currentNode = root;
                }
                bitCount++;
            }
        }
        return decodedData;
    }

    void HuffDecompressor::writeDecodedData(const string& input_file_name, const vector<uint8_t> &decodedData) {
        std::ofstream outputFile(input_file_name, std::ios::binary);
        if (!outputFile.is_open()) {
            std::cerr << "Error opening output file: " << input_file_name << std::endl;
            exit(1);
        }
        outputFile.write(reinterpret_cast<const char*>(decodedData.data()), decodedData.size());
        outputFile.close();
        std::cout << "Decoded data written to: " << input_file_name << std::endl;
    }

    void HuffDecompressor::destroyTree(HuffmanNode *root) {
        if (!root) return;

        destroyTree(root->left);
        destroyTree(root->right);
        delete root;
    }
}