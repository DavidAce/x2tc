cmake_minimum_required(VERSION 3.24)
project(x2tc-consumer LANGUAGES CXX)

find_package(x2tc REQUIRED)
add_executable(x2tc-consumer main.cpp)
target_link_libraries(x2tc-consumer PRIVATE x2tc::x2tc)

