#######################################
# Google Mock
add_subdirectory("${gmock_dir}" "${QtTest_BINARY_DIR}/gmock" EXCLUDE_FROM_ALL)

include_directories("${gmock_dir}/include")
include_directories("${gmock_dir}/gtest/include")

#######################################
# Test list
enable_testing()

aux_source_directory(. KRAKEN_TEST_SRC_LIST)
set( kraken_test_LIBS kraken gmock_main)

kcxx_test(test-all "${kraken_test_LIBS}" test-all.cc CodeChunkHelper.cpp)
