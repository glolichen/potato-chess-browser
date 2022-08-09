cmake_minimum_required(VERSION 3.0.0)
project(potato-chess VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")

add_executable(potato-chess bitboard.cpp eval.cpp main.cpp maps.cpp move.cpp moveGen.cpp perft.cpp search.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
