message ("---------------------------")
message ("| Message code generation |")
message ("---------------------------")

file (GLOB MSGS_GEN ${CMAKE_CURRENT_SOURCE_DIR}/msg/*.msg)

set(TMP_MSG_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/msg/src)
file(MAKE_DIRECTORY ${TMP_MSG_SRC_DIR})
foreach(file ${MSGS_GEN})
	message("Generating C++ code for ${file}")
	execute_process(COMMAND racket ${CMAKE_CURRENT_SOURCE_DIR}/msg_generation/message_generator.rkt ${file} ${TMP_MSG_SRC_DIR}
					RESULT_VARIABLE result)
	message(${result}) 
endforeach()

file (GLOB MSGS_SRC ${TMP_MSG_SRC_DIR}/*.cpp)
file (GLOB MSGS_HEADERS ${TMP_MSG_SRC_DIR}/*.h)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/msg)
foreach(file ${MSGS_SRC})
	configure_file(${file} ${CMAKE_CURRENT_SOURCE_DIR}/src/msg)
endforeach()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/com/msg)
foreach(file ${MSGS_HEADERS})
	configure_file(${file} ${CMAKE_CURRENT_SOURCE_DIR}/include/com/msg)
endforeach()

file(REMOVE_RECURSE ${TMP_MSG_SRC_DIR})