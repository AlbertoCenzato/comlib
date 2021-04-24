find_package (GTest REQUIRED)

if (MSVC)
	set (SYSTEM_DEPENDENT_LIBS "")
else (MSVC)
	set (SYSTEM_DEPENDENT_LIBS pthread)
endif (MSVC)

include_directories (. ${GTEST_INCLUDE_DIRS})

# Mocks
file(GLOB MOCKS_SRC mocks/*.cpp)

add_library (mocks ${MOCKS_SRC})
target_link_libraries (mocks PRIVATE comlib ${SYSTEM_DEPENDENT_LIBS})

# Unit tests
file(GLOB UNIT_TEST_SRC unit_tests/*.cpp)

add_executable (comlib_unit_tests ${UNIT_TEST_SRC})
target_link_libraries(comlib_unit_tests PRIVATE comlib mocks ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${SYSTEM_DEPENDENT_LIBS}) #GTest::gtest GTest::gtest_main)

gtest_add_tests(TARGET comlib_unit_tests)

# Integration tests
file(GLOB INTEGRATION_TEST_SRC integration_tests/*.cpp)

add_executable (comlib_integration_tests ${INTEGRATION_TEST_SRC})
target_link_libraries(comlib_integration_tests PRIVATE comlib mocks ${SYSTEM_DEPENDENT_LIBS})