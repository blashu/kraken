cmake_minimum_required(VERSION 2.8)

file(GLOB SIMPLE_EXE_HEADERS *.h)
aux_source_directory(. SIMPLE_EXE_SRC_LIST)
add_executable(simple_exe ${SIMPLE_EXE_SRC_LIST} ${SIMPLE_EXE_HEADERS})
