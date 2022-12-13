cmake_minimum_required(VERSION 3.22)
project(Bookstore)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(Bookstore
        src/BlockLinkedList.cpp
        src/BlockLinkedList.h)
