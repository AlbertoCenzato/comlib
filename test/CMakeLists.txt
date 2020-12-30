find_package (GTest REQUIRED)

if (MSVC)
	set (SYSTEM_DEPENDENT_LIBS "")
else (MSVC)
	set (SYSTEM_DEPENDENT_LIBS pthread)
endif (MSVC)

include_directories (. ../src ${GTEST_INCLUDE_DIRS})

# Unit tests
set (UNIT_TEST_SRC
	unit_tests_main.cpp
	communication.cpp
	utils.cpp
	mock_message_socket.cpp
	loopback_mock_message_socket.cpp
	stream_message_reader.cpp
)

add_executable (comlib_unit_tests ${UNIT_TEST_SRC})
target_link_libraries(comlib_unit_tests PRIVATE comlib ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${SYSTEM_DEPENDENT_LIBS}) #GTest::gtest GTest::gtest_main)

gtest_add_tests(TARGET comlib_unit_tests)

# Integration tests
set (INTEGRATION_TEST_SRC
	integration_tests_main.cpp
	mock_message_socket.cpp
	loopback_mock_message_socket.cpp
)

add_executable (comlib_integration_tests ${INTEGRATION_TEST_SRC})
target_link_libraries(comlib_integration_tests PRIVATE comlib ${SYSTEM_DEPENDENT_LIBS})