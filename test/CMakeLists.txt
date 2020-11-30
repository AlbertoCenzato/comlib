find_package (GTest REQUIRED)

if (MSVC)
	set (SYSTEM_DEPENDENT_LIBS "")
else (MSVC)
	set (SYSTEM_DEPENDENT_LIBS pthread)
endif (MSVC)


include_directories (../src ${GTEST_INCLUDE_DIRS})

file (GLOB_RECURSE TEST_SRC *.cpp)

add_executable (comlib_unit_tests ${TEST_SRC})
target_link_libraries(comlib_unit_tests PRIVATE comlib ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} ${SYSTEM_DEPENDENT_LIBS}) #GTest::gtest GTest::gtest_main)

gtest_add_tests(TARGET comlib_unit_tests)