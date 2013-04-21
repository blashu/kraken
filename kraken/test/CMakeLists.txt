#######################################
# Google Mock
add_subdirectory("${gmock_dir}" "${QtTest_BINARY_DIR}/gmock" EXCLUDE_FROM_ALL)

include_directories("${gmock_dir}/include")
include_directories("${gmock_dir}/gtest/include")

#######################################
# Test list
enable_testing()

file(GLOB KRAKEN_TEST_HEADERS *.h)
aux_source_directory(. KRAKEN_TEST_SRC)

set( kraken_test_LIBS kraken gmock_main)

kcxx_test(test-all "${kraken_test_LIBS}" ${KRAKEN_TEST_HEADERS} ${KRAKEN_TEST_SRC})
