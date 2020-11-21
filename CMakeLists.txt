cmake_minimum_required (VERSION 3.10)

project ("comlib")

file (GLOB_RECURSE SRC src/*.cpp) 
add_library(comlib ${SRC})

target_include_directories(comlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (MSVC)
	target_compile_options( comlib PRIVATE /arch:AVX2 /WX /EHsc )  # enable AVX2 SIMD instruction set; use latest c++ standard; treat warnings as errors
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++20")
else (MSVC)
	target_compile_options( comlib PRIVATE -Werror -mavx2)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
endif (MSVC)

#enable_testing()
#add_subdirectory(test)