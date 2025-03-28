#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <filesystem>

using std::string;
using std::vector;

namespace Compressor {

    class ImageCompressor {

        public:
            ImageCompressor() {}
            
            void compress(const std::filesystem::path& filePath);

        private:
            cv::Mat image;
    };
}