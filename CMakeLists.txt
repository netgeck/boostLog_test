project( test )
cmake_minimum_required(VERSION 2.8)


SET( PROGRAM_NAME "Test" )


# Устанавливаем переменные чтобы бинарники падали в корень сборочной директории
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} )

add_definitions( -Wunused -Wall -Wextra )
list( APPEND CMAKE_CXX_FLAGS "-std=c++0x" )
#list( APPEND CMAKE_CXX_FLAGS "-std=c++11" )

# каталоги инклюдов
include_directories(src)
include_directories(libfdevent)


# Программы
add_subdirectory(src)

