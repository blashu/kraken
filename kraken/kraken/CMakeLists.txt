aux_source_directory(. KRAKEN_SRC_LIST)

add_library(kraken ${KRAKEN_SRC_LIST})
target_link_libraries(kraken BeaEngine)
