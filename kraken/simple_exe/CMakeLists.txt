cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SIMPLE_EXE_SRC_LIST)
add_executable(simple_exe "${SIMPLE_EXE_SRC_LIST}" )
