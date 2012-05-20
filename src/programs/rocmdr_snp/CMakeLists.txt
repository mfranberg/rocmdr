find_package( PlinkIo REQUIRED )
find_package( GenGetOpt REQUIRED )

include_directories( ${PLINKIO_INCLUDE_DIR} )

include_directories( "${CMAKE_CURRENT_BINARY_DIR}" )

include_directories( ${LIBROCMDR_INCLUDE_DIR} )
include_directories( ${DENSE_HASHMAP_INCLUDE_DIR} )
include_directories( ${LIBCSV_INCLUDE_DIR} )
include_directories( ${CPPTHREAD_INCLUDE_DIR} )

run_ggo( GGO_SRC rocmdr_snp.ggo )

add_executable( rocmdr_snp rocmdr_snp.cpp plink/PlinkIo.cpp RestrictReader.cpp PairReader.cpp ${GGO_SRC} )
target_link_libraries( rocmdr_snp librocmdr libcppthread ${PLINKIO_LIBRARIES} ${PTHREADS_LIBRARIES} )
