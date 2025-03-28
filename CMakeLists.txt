cmake_minimum_required(VERSION 3.10)

# Project name and language
project(FileCompressor)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/src/include)

# CMAKE_RUNTIME_OUTPUT_DIRECTORY is where executables (like .exe files) go.
# CMAKE_LIBRARY_OUTPUT_DIRECTORY is for shared libraries (.dll).
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY is for static libraries (.lib or .a).
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "C:/Tools/fcmp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "C:/Tools/fcmp")

# Ensure the directory exists
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(fcmp
    src/main.cpp
    src/utils/utils.cpp
    src/compressor/compressor.cpp
    src/compressor/decompressor.cpp
    src/compressor/images.cpp
)

# add dependencies for opencv library
# have to manually set the library location and link libraries to it
set(OpenCV_DIR "C:/Tools/opencv-mingw")
find_package(OpenCV REQUIRED)

if(OpenCV_FOUND)
    message(STATUS "OpenCV found: ${OpenCV_VERSION}")
    target_link_libraries(fcmp PRIVATE ${OpenCV_LIBS})
    # use opencv definitions (ie check it exists before running functions)
    target_compile_definitions(fcmp PRIVATE USE_OPENCV)
else()
    message(WARNING "OpenCV not found. Image Compression is disabled.")
endif()

add_custom_target(
    # create a new target called run, ALL lets it run when the project is built but not used
    run
    # what to run when target is built
    COMMAND FileCompressor.exe compress ../tests/test.txt
    # tells to cmake to make sure FileCompressor target is already built
    DEPENDS FileCompressor ${PROJECT_SOURCE_DIR}/tests/test.txt
    # set working directory where the command runs
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)