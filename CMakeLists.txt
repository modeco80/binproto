cmake_minimum_required(VERSION 3.17)
project(binproto)

# TODO clean this up and upgrade it to my standards

set(CMAKE_CXX_STANDARD 20)

add_library(binproto src/EndianUtils.cpp src/BufferReader.cpp src/BufferWriter.cpp)
target_include_directories(binproto PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(test src/test/main.cpp)
target_link_libraries(test binproto)