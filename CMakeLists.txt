cmake_minimum_required (VERSION 3.10)

project ("comlib")

file (GLOB MSGS_GEN ${CMAKE_CURRENT_LIST_DIR}/msg/*.msg)

set(TMP_MSG_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/msg/src)
file(MAKE_DIRECTORY ${TMP_MSG_SRC_DIR})
foreach(file ${MSGS_GEN})
	message("Generating C++ code for ${file}")
	execute_process(COMMAND racket ${CMAKE_CURRENT_LIST_DIR}/msg_generation/message_generator.rkt ${file} ${TMP_MSG_SRC_DIR}
					RESULT_VARIABLE result)
	message(${result}) 
endforeach()

file (GLOB MSGS_SRC ${TMP_MSG_SRC_DIR}/*.cpp)
file (GLOB MSGS_HEADERS ${TMP_MSG_SRC_DIR}/*.h)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src/msg)
foreach(file ${MSGS_SRC})
	configure_file(${file} ${CMAKE_CURRENT_LIST_DIR}/src/msg)
endforeach()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/com/msg)
foreach(file ${MSGS_HEADERS})
	configure_file(${file} ${CMAKE_CURRENT_LIST_DIR}/include/com/msg)
endforeach()

file(REMOVE_RECURSE ${TMP_MSG_SRC_DIR})


file (GLOB SRC src/*.cpp)  
file (GLOB HEADERS include/com/*.h)

file (GLOB MSGS_SRC src/msg/*.cpp)
file (GLOB MSGS_HEADERS include/com/msg/*.h)

if (ARDUINO)
	message("Sto compilando per arduino") 
	# TODO: clear directory before copy
	set(LIB_DIR ${ARDUINO_LIBS_PATH}/${PROJECT_NAME})
	file(REMOVE_RECURSE ${LIB_DIR})
	file(MAKE_DIRECTORY ${LIB_DIR})
	file (COPY ${SRC} DESTINATION ${LIB_DIR})
	file (COPY ${HEADERS} DESTINATION ${LIB_DIR}/com)
	file (COPY ${LIB_DIR}/com/comlib.h DESTINATION ${LIB_DIR}) # NOTE: move comlib to toplevel dir
	file (COPY ${MSGS_SRC} DESTINATION ${LIB_DIR})  # NOTE: source file should be all in the same directory
	file (COPY ${MSGS_HEADERS} DESTINATION ${LIB_DIR}/com/msg)  # NOTE: workaroud to make includes visible by all files
	return()
endif (ARDUINO)

add_library (comlib 
	${SRC}
	${MSGS_SRC}
)
target_include_directories(comlib PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/include
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