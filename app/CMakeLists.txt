cmake_minimum_required(VERSION 3.20)
project(VAGEDC15Logger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APP_VERSION MATCHES "([0-9]+)\\.([0-9]+)\\.([0-9]+)(\\.([0-9]+))?")
    set(V_MAJOR ${CMAKE_MATCH_1})
    set(V_MINOR ${CMAKE_MATCH_2})
    set(V_PATCH ${CMAKE_MATCH_3})
    if(CMAKE_MATCH_5)
        set(V_BUILD ${CMAKE_MATCH_5})
    else()
        set(V_BUILD 0)
    endif()
else()
    set(V_MAJOR 0)
    set(V_MINOR 0)
    set(V_PATCH 0)
    set(V_BUILD 0)
endif()

configure_file(
    ./version.h.in
    ${CMAKE_BINARY_DIR}/generated/version.h
    @ONLY)

add_executable(VAGEDC15Checksum VAGEDC15Checksum.cpp)

if(WIN32)
	target_sources(VAGEDC15Checksum PRIVATE VAGEDC15Checksum.rc)
endif()


target_include_directories(VAGEDC15Checksum PRIVATE . ${CMAKE_BINARY_DIR}/generated)
target_link_libraries(VAGEDC15Checksum PRIVATE vagedc15core VAGEDC15ChecksumCore)





