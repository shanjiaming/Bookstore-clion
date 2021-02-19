cmake_minimum_required(VERSION 3.15)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp UnrolledLinkedList.hpp)
#add_executable(testmain testmain.cpp)
#add_executable(testUnrolledLinkedList testUnrolledLinkedList.cpp
#        UnrolledLinkedList.hpp)
#add_executable(testchar testchar.cpp)