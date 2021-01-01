find_package (GTest REQUIRED)

if (MSVC)
	set (SYSTEM_DEPENDENT_LIBS "")
else (MSVC)
	set (SYSTEM_DEPENDENT_LIBS pthread)
endif (MSVC)

include_directories (. ${GTEST_INCLUDE_DIRS})

# Mocks
set (MOCKS_SRC
	mocks/mock_message_socket.cpp
)

add_library (mocks ${MOCKS_SRC})
target_link_libraries (mocks PRIVATE comlib ${SYSTEM_DEPENDENT_LIBS})

# Unit tests
set (UNIT_TEST_SRC
	unit_tests/unit_tests_main.cpp
	unit_tests/communication.cpp
	unit_tests/utils.cpp
	unit_tests/mock_message_socket.cpp
	unit_tests/stream_message_reader.cpp
)

add_executable (comlib_unit_tests ${UNIT_TEST_SRC})
target_link_libraries(comlib_unit_tests PRIVATE comlib mocks ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${SYSTEM_DEPENDENT_LIBS}) #GTest::gtest GTest::gtest_main)

gtest_add_tests(TARGET comlib_unit_tests)

# Integration tests
set (INTEGRATION_TEST_SRC
	integration_tests/integration_tests_main.cpp
)

add_executable (comlib_integration_tests ${INTEGRATION_TEST_SRC})
target_link_libraries(comlib_integration_tests PRIVATE comlib mocks ${SYSTEM_DEPENDENT_LIBS})