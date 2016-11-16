cmake_minimum_required(VERSION 3.5)
project(NMF_for_cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp NMF.hpp Array2D.hpp Separator.h)
add_executable(NMF_for_cpp ${SOURCE_FILES})