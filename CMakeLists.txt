cmake_minimum_required(VERSION 3.10)

# Project name and language
project(FileCompressor)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/src/include)

add_executable(FileCompressor
    src/main.cpp
    src/utils/utils.cpp
    src/compressor/compressor.cpp
)

add_custom_target(
    # create a new target called run, ALL lets it run when the project is built
    run
    # what to run when target is built
    COMMAND FileCompressor.exe ${PROJECT_SOURCE_DIR}/tests/test.txt
    # tells to cmake to make sure FileCompressor target is already built
    DEPENDS FileCompressor ${PROJECT_SOURCE_DIR}/tests/test.txt
    # set working directory where the command runs
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_custom_target(
    # create a new target called run
    one
    # what to run when target is built
    COMMAND FileCompressor.exe input.txt
    # tells to cmake to make sure FileCompressor target is already built
    DEPENDS FileCompressor
    # set working directory where the command runs
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)

add_custom_target(
    # create a new target called run
    rubbish
    # what to run when target is built
    COMMAND FileCompressor.exe 
    # tells to cmake to make sure FileCompressor target is already built
    DEPENDS FileCompressor
    # set working directory where the command runs
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)