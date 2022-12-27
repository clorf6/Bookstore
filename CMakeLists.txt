cmake_minimum_required(VERSION 3.16)
project(Bookstore)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(code src/main.cpp src/Utils.cpp src/AccountSystem.cpp src/BookSystem.cpp src/Bookstore.cpp)
