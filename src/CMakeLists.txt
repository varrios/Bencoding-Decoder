cmake_minimum_required(VERSION 3.28)
project(src)

set(CMAKE_CXX_STANDARD 17)

add_executable(src main.cpp
        bencoding_decoder.cpp
        bencoding_decoder.h)
