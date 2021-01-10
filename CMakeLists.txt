cmake_minimum_required (VERSION 3.10)

project ("comlib")

file (GLOB SRC src/com/*.cpp) 
file (GLOB HEADERS src/com/*.h)

file (GLOB MSGS_SRC src/com/msg/*.cpp)
file (GLOB MSGS_HEADERS src/com/msg/*.h)

if (ARDUINO)
	message("Sto compilando per arduino")
	file (COPY ${SRC} DESTINATION ${ARDUINO_LIBS_PATH}/${PROJECT_NAME})
	file (COPY ${HEADERS} DESTINATION ${ARDUINO_LIBS_PATH}/${PROJECT_NAME})
	file (COPY ${MSGS_SRC} DESTINATION ${ARDUINO_LIBS_PATH}/${PROJECT_NAME}/msg)
	file (COPY ${MSGS_HEADERS} DESTINATION ${ARDUINO_LIBS_PATH}/${PROJECT_NAME}/msg)
	return()
endif (ARDUINO)

add_library (comlib 
	${SRC}
	${MSGS_SRC}
)
target_include_directories(comlib PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (MSVC)
	target_compile_options( comlib PRIVATE /arch:AVX2 /WX /EHsc )  # enable AVX2 SIMD instruction set; use latest c++ standard; treat warnings as errors
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
else (MSVC)
	target_compile_options( comlib PRIVATE -Werror)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
endif (MSVC)

enable_testing()
add_subdirectory(test)