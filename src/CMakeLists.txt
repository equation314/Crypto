cmake_minimum_required(VERSION 3.1)
project(Crypto)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 -O3 -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(SRCS bytearray.cpp aes.cpp sha3.cpp utils.h)
add_library(crypto ${SRCS})

add_executable(aes mainaes.cpp)
target_link_libraries(aes crypto)

add_executable(sha3 mainsha3.cpp)
target_link_libraries(sha3 crypto)

add_executable(crypto_test test.cpp)
target_link_libraries(crypto_test crypto)
