#include "compressor.h"
#include "utils.h"

namespace Compressor {
    using std::priority_queue;

    /// @brief Compression program
    /// @param input 
    /// @param outputFilePath 
    void HuffCompressor::compress(const std::filesystem::path& inputFilePath, const vector<uint8_t> file_input) {
        // Bring everything together

        // File has been read in main program - Create the frequency table
        buildFrequencyTable(file_input);

        // Build the Huffman tree from the frequency table
        root = buildHuffmanTree(std::nullopt);

        // Generate the Huffman codes for each character in the frequency table
        generateHuffmanCodes(root, {});

        // Encode the data using the Huffman codes
        pair<vector<uint8_t>, int> encodedData = encodeData(file_input);

        // Write the compressed data to a file
        writeCompressedData(inputFilePath, encodedData);
    }

    void HuffCompressor::printHuffmanTree(HuffmanNode* node, const std::string& code) {
        if (!node) return;
    
        // If this is a leaf node (no children)
        if (!node->left && !node->right) {
            std::cout << "Symbol: " << static_cast<int>(node->data)
                      << " ('" << (char)(isprint(node->data) ? node->data : '.') << "')"
                      << " | Frequency: " << node->frequency
                      << " | Code: " << code << std::endl;
            return;
        }
    
        // Traverse left and right
        printHuffmanTree(node->left, code + "0");
        printHuffmanTree(node->right, code + "1");
    }

    /// @brief  Creates a map of each character and how often they appear in the input
    /// @param input - file data converted to a byte vector. 
    void HuffCompressor::buildFrequencyTable(const vector<uint8_t>& input) {
        for (const uint8_t &byte : input) {
            frequencyTable[byte]++;
        }
    }
    
    /// @brief Builds a huffman tree from a frequency table
    /// Using a priority queue - base elements based on their priority
    /// We would be using min-heap for huffman encoding - heap where the smallest element (lowest priority) comes out first
    /// repeatedly merge the two least frequent nodes
    HuffmanNode* HuffCompressor::buildHuffmanTree(const std::optional<unordered_map<uint8_t, int>>&
         receivedFrequencyTable) {
        // First create queue & insert all elements into the queue. The queue will automatically reorder based on frequency
        /**
         *  std::priority_queue<
                Type,                      // The type of elements stored in the queue
                Container,                 // The underlying container used to store elements (usually std::vector)
                Compare                    // The comparison function or functor that controls priority
            > variable_name;
         */
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, HuffmanCompare> min_heap;

        // sort frequency table before inserting into min_heap - solved decompression problem - guarantees consistency when inserting since we'll always insert in the same order
        std::map<uint8_t, int> sortedFrequencyTable(receivedFrequencyTable.value_or(frequencyTable).begin(), 
                                                    receivedFrequencyTable.value_or(frequencyTable).end());

        for (const auto &entry : sortedFrequencyTable) {
            min_heap.push(new HuffmanNode(entry.first, entry.second));
        }
 
        // Build the actual Huffman tree
        while (min_heap.size() > 1) {
            // pop the two nodes with the smallest priorities
            HuffmanNode *left = min_heap.top();
            min_heap.pop();
            HuffmanNode *right = min_heap.top();
            min_heap.pop();

            // Create a combined node for the two frequencies and push back into the queue
            // You don't need valid data in internal (combined) nodes in a huffman tree so it's often intialized with 0 or nothing
            // Their purpose is to connect the leaf nodes and represent the combined frequency of their children
            HuffmanNode *combined = new HuffmanNode(0, left->frequency + right->frequency);
            combined->left = left;
            combined->right = right;

            // Push the combined node back into the queue
            min_heap.push(combined);
        }
        if (min_heap.empty()) {
            throw std::runtime_error("Error: Huffman tree construction failed. Priority queue is empty.");
        } 
        return min_heap.top();
    }

    /// @brief Assign a unique binary code to each character by traversing the tree
    /// @param node root to start movement from
    /// @param code string that contains the code - gets appended while traversing the tree
    void HuffCompressor::generateHuffmanCodes(HuffmanNode *node, const string& code) {
        if (!node) return;

        // Check if node is leaf - it has no left or right children
        if (!node->left &&!node->right) {
            huffmanCodes[node->data] = code;
        }

        // Traverse through tree
        //0 when you go left
        generateHuffmanCodes(node->left, code + "0");
        //1 when you go right
        generateHuffmanCodes(node->right, code + "1");
    }

    /// @brief Encodes the original file data into huffman code - ready for writing 
    /// @param data original file data to map byte to huffman code
    /// @return bitstring of huffman code
    pair<vector<uint8_t>, int> HuffCompressor::encodeData(const vector<uint8_t> &data) {
        string bitString;
        for (auto byte: data) {
            string code = huffmanCodes[byte];
            for (auto bit : code) {
                bitString += (bit == '1') ? '1' : '0';
            }
        }

        // Iterate over bitString and pack it into bytes
        std::vector<uint8_t> byteArray;

        size_t length = bitString.length();
        uint8_t currentByte = 0;
        int bitCount = 0;

        for (char bit : bitString) {
            // Shift left to make room for the new bit
            // If the current bit is '1', set the least significant bit
            currentByte = (currentByte << 1) | (bit - '0');

            bitCount++;

            // If we have collected 8 bits, push it to the array
            if (bitCount == 8) {
                byteArray.push_back(currentByte);
                currentByte = 0;
                bitCount = 0;
            }
        }

        // Handle the last byte if it doesn't have 8 bits
        if (bitCount > 0) {
            // Shift the remaining bits to the left to fill the byte
            currentByte <<= (8 - bitCount);
            byteArray.push_back(currentByte);
        }

        // Return the byte array and the total number of bits
        return pair<vector<uint8_t>, int>(byteArray, length);
    }

    /// @brief Function to write the compressed data to an output file
    /// @param outputFilePath Path to the output file 
    /// @param encodedData Huffman encoded data and the original size of the bitString
    void HuffCompressor::writeCompressedData(const std::filesystem::path& inputFilePath, pair<vector<uint8_t>, int>& encodedData) {
        // Layout of output file looks like this:
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
        vector<uint8_t> outputFileBuffer;

        // Write original file name size
        uint32_t nameSize = inputFilePath.filename().string().size();
        Utils::appendToBuffer(outputFileBuffer, nameSize);

        // Write original file name (the original extension is included)
        string filename = inputFilePath.filename().string();
        outputFileBuffer.insert(outputFileBuffer.end(), filename.begin(), filename.end());

        // Write tableSize
        uint32_t tableSize = static_cast<uint32_t>(frequencyTable.size());
        Utils::appendToBuffer(outputFileBuffer, tableSize);

        // Write each frequency table entry
        for (const auto &entry : frequencyTable) {
            Utils::appendToBuffer(outputFileBuffer, entry.first);
            Utils::appendToBuffer(outputFileBuffer, entry.second);
        }

        // Write totalBits
        uint32_t totalBits = static_cast<uint32_t>(encodedData.second);
        Utils::appendToBuffer(outputFileBuffer, totalBits);

        // Write compressed data
        outputFileBuffer.insert(outputFileBuffer.end(), encodedData.first.begin(), encodedData.first.end());

        // Generate the output file name and Write the output file
        // Create the output file path
        string parent_path = inputFilePath.parent_path().string();
        string outputFilePath = "";
        if (parent_path == ".") {
            outputFilePath = inputFilePath.stem().string() + "_compressed.fcm";
        } else {
            outputFilePath = parent_path + "\\" + inputFilePath.stem().string() + "_compressed.fcm";
        }
        Utils::writeFile(outputFilePath, outputFileBuffer);

        std::cout << "Done. " << std::endl;
    }
            
    void HuffCompressor::destroyTree(HuffmanNode *root) {
        if (!root) return;

        destroyTree(root->left);
        destroyTree(root->right);
        delete root;
    }
    
}