project( rocmdr )
cmake_minimum_required( VERSION 2.7 )

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/.cmake/modules/" )

find_package( Pthreads )

set( ROCMDR_VERSION_MAJOR 0 )
set( ROCMDR_VERSION_MINOR 1 )
set( ROCMDR_VERSION_PATCH 0 )

set( LIBCSV_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/libs/libcsv/inc/" )
set( DENSE_HASHMAP_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/libs/dense_hashmap/" )
set( LIBROCMDR_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/libs/librocmdr/inc/" )
set( LIBCPPTHREAD_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/libs/libcppthread/" )

enable_testing( )
set( CXXTEST_PERL_TESTGEN_EXECUTABLE "${PROJECT_SOURCE_DIR}/libs/libcxxtest/cxxtestgen.pl" )
set( CXXTEST_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/libs/libcxxtest/" )

add_subdirectory( libs )
add_subdirectory( programs )
add_subdirectory( unittests )
