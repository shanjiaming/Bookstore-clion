cmake_minimum_required(VERSION 3.15)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp
        UnrolledLinkedList.h UnrolledLinkedList.cpp
        BookAndUser.h BookAndUser.cpp
        BasicHeader.h
        Data.h Data.cpp
        logger.h filemanip.h filemanip.cpp)
#add_executable(testmain testmain.cpp)
#add_executable(testUnrolledLinkedList testUnrolledLinkedList.cpp
#        UnrolledLinkedList.h UnrolledLinkedList.cpp)
#add_executable(testchar testchar.cpp)