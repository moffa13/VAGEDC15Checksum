cmake_minimum_required(VERSION 3.20)
project(VAGEDC15Checksum LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(VAGEDC15Lib
    GIT_REPOSITORY https://github.com/moffa13/VAGEDC15Lib.git
    GIT_TAG main)
FetchContent_MakeAvailable(VAGEDC15Lib)



add_subdirectory(src)
add_subdirectory(app)

include(CTest)
if(BUILD_TESTING AND PROJECT_IS_TOP_LEVEL)
	add_subdirectory(tests)
endif()
