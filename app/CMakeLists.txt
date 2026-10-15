cmake_minimum_required(VERSION 3.20)
project(VAGEDC15Logger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(VAGEDC15Checksum VAGEDC15Checksum.cpp)

target_include_directories(VAGEDC15Checksum PRIVATE .)
target_link_libraries(VAGEDC15Checksum PRIVATE vagedc15core VAGEDC15ChecksumCore)





