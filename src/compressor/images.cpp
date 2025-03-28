#include "images.h"

namespace Compressor {

    /// @brief Compress image sizes
    /// @param filePath Path to the image to compress
    void ImageCompressor::compress(const std::filesystem::path& filePath) {
        // imread takes a filename
        string fileName = filePath.filename().string();
        image = cv::imread(filePath.string());
        if (!image.data) {
            std::cerr << "Error: Could not open or find the image." << std::endl;
            return;
        }

        // Set jpeg compression quality (1-100, lower = higher compression)
        vector<int> compression_params;
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(15);

        string compressedFilename = filePath.stem().string() + "_compressed.jpg";
        cv::imwrite(compressedFilename, image, compression_params);

        std::cout << "Compressed image saved as: " << compressedFilename << std::endl;
        std::cout << "Done." << std::endl;
    }

}