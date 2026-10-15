add_library(VAGEDC15ChecksumCore STATIC
	EDC15/EDC15Checksum.cpp
	Core/BitArithmetics.cpp)

target_include_directories(VAGEDC15ChecksumCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(VAGEDC15ChecksumCore PRIVATE vagedc15core)

