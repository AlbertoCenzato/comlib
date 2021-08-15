message ("---------------------------------------------")
message ("| Installing ${PROJECT_NAME} as Arduino lib |")
message ("---------------------------------------------")

set (LIB_DIR ${ARDUINO_LIBS_PATH}/${PROJECT_NAME})
file (REMOVE_RECURSE ${LIB_DIR})
file (MAKE_DIRECTORY ${LIB_DIR})

file (COPY ${SRC} DESTINATION ${LIB_DIR})
file (COPY ${HEADERS} DESTINATION ${LIB_DIR}/com)

file (COPY ${MSGS_SRC} DESTINATION ${LIB_DIR})  # NOTE: source file should be all in the same directory
file (COPY ${MSGS_HEADERS} DESTINATION ${LIB_DIR}/com/msg)

file (APPEND ${LIB_DIR}/comlib.h "#pragma once\n\n")
foreach (file ${HEADERS})
	get_filename_component (FILE_NAME ${file} NAME)
	file (APPEND ${LIB_DIR}/comlib.h "#include \"com/${FILE_NAME}\"\n")
endforeach()

foreach (file ${MSGS_HEADERS})
	get_filename_component (FILE_NAME ${file} NAME)
	file (APPEND ${LIB_DIR}/comlib.h "#include \"com/msg/${FILE_NAME}\"\n")
endforeach()