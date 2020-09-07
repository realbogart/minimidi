cmake_minimum_required(VERSION 3.17)

add_library(minimidi STATIC "")

target_include_directories (minimidi 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_sources(minimidi 
	PUBLIC 
		"${CMAKE_CURRENT_LIST_DIR}/include/minimidi.h" 
		"${CMAKE_CURRENT_LIST_DIR}/include/minimidi_reader.h" 
	PRIVATE 
		"${CMAKE_CURRENT_LIST_DIR}/source/minimidi.cpp"
		"${CMAKE_CURRENT_LIST_DIR}/source/minimidi_reader.cpp"
)