#pragma once

#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <filesystem>
#include "utils.h"
#include "compressor.h"

namespace Decompressor {

    using std::vector;
    using std::string;
    using std::unordered_map;
    using std::pair;
    using HuffmanNode = Compressor::HuffmanNode;
    using HuffmanCompare = Compressor::HuffmanCompare;

    class HuffDecompressor {

        public:
            HuffDecompressor() : root(nullptr) {}
            ~HuffDecompressor() {
                // Since it's called in destructor, no need to call explicitly
                destroyTree(root);
            }

            void decompress (const std::filesystem::path& inputFilePath);

        private:
            vector<uint8_t> decodeCompressedData(const uint32_t &totalBits, const vector<uint8_t> &compressedData);
            void writeDecodedData(const string& input_file_name, const vector<uint8_t> &decodedData);
            void destroyTree(HuffmanNode *root);

            unordered_map<uint8_t, int> frequencyTable;
            HuffmanNode* root = nullptr;
    };
}