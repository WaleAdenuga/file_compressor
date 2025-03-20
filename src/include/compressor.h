#pragma once

#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
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
    struct HuffmanCompare {
        bool operator()(HuffmanNode *a, HuffmanNode *b) {
            return a->frequency > b->frequency;
        }
    };

    class HuffCompressor {

        public:
            HuffCompressor() : root(nullptr) {}
            ~HuffCompressor() {
                // Since it's called in destructor, no need to call explicitly
                destroyTree(root);
            }

            void compress (const vector<uint8_t> input, const string& outputFilePath);

        private:
            void buildFrequencyTable(const vector<uint8_t>& input);
            void buildHuffmanTree();
            void generateHuffmanCodes(HuffmanNode *node, const vector<bool>& code);
            pair<vector<uint8_t>, int> encodeData(const vector<uint8_t> &data);
            void writeCompressedData(const string& outputFilePath, pair<vector<uint8_t>, int>& encodedData);
            void destroyTree(HuffmanNode *root);

            unordered_map<uint8_t, int> frequencyTable;
            unordered_map<uint8_t, vector<bool>> huffmanCodes;
            HuffmanNode* root = nullptr;
    };
}