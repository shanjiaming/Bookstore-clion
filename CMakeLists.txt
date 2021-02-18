cmake_minimum_required(VERSION 3.15)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp HashTable.hpp)
add_executable(testmain testmain.cpp)
add_executable(testUnrolledLinkedList testUnrolledLinkedList.cpp
        HashTable.hpp)