#pragma once

#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <map>
#include <filesystem>
#include <optional>
#include <iostream>
#include <cstdint>
#include <algorithm>
#include "utils.h"

namespace Compressor {

    using std::vector;
    using std::string;
    using std::unordered_map;
    using std::pair;

    struct HuffmanNode {
        uint8_t data;
        int frequency;
        HuffmanNode* left = nullptr;
        HuffmanNode* right = nullptr;

        // struct constructor - to initialize
        HuffmanNode(uint8_t data, int frequency) : data(data), frequency(frequency) {}
    };

    // functor - acts like a function when you use the () operator
    // called by prioriy queue to compare two nodes to determine which has higher priority
    // huffman needs a min-heap where smallest node has the highest priority
    // priority_queue in c++ is max-heap
    // element a has higher priority if a->frequency is smaller than b->frequency
    // What do we do when the frequency is the same (and screws up the tree generating different codes)? 
    //         We compare by their a unique id to the huffman node
    struct HuffmanCompare {
        bool operator()(HuffmanNode *a, HuffmanNode *b) {
            if (a->frequency != b->frequency)  return a->frequency > b->frequency; // Tie-breaker: Sort by data value
            return a->data > b->data; // Default: Sort by frequency
        }
    };

    class HuffCompressor {

        public:
            explicit HuffCompressor() : root(nullptr) {}
            ~HuffCompressor() {
                // Since it's called in destructor, no need to call explicitly
                destroyTree(root);
            }

            void compress (const std::filesystem::path& outputFilePath, const vector<uint8_t> file_input);
            HuffmanNode* buildHuffmanTree(const std::optional<unordered_map<uint8_t, int>>& receivedFrequencyTable);
            void printHuffmanTree(HuffmanNode* node, const std::string& code);

        private:
            void buildFrequencyTable(const vector<uint8_t>& input);     
            void generateHuffmanCodes(HuffmanNode *node, const string& code);
            pair<vector<uint8_t>, int> encodeData(const vector<uint8_t> &data);
            void writeCompressedData(const std::filesystem::path& input_file_path, pair<vector<uint8_t>, int>& encodedData);
            void destroyTree(HuffmanNode *root);  

            unordered_map<uint8_t, int> frequencyTable;
            unordered_map<uint8_t, string> huffmanCodes;
            HuffmanNode* root = nullptr;
    };
}