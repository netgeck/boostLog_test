project(testBoostLog)
cmake_minimum_required(VERSION 3.0)

add_definitions(-Wunused -Wall -Wextra)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

set(SHARED_LIB_DIR ${CMAKE_BINARY_DIR}) # в корень сборочной директории

# Программы
add_subdirectory(src)