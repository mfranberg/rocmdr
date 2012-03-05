find_package( GenGetOpt REQUIRED )

include_directories( "${CMAKE_CURRENT_BINARY_DIR}" )

include_directories( ${LIBROCMDR_INCLUDE_DIR} )
include_directories( ${DENSE_HASHMAP_INCLUDE_DIR} )
include_directories( ${LIBCSV_INCLUDE_DIR} )

run_ggo( GGO_SRC rocmdr.ggo )

add_executable( rocmdr rocmdr.cpp parsing.cpp ${GGO_SRC} )
target_link_libraries( rocmdr librocmdr libcsv )
