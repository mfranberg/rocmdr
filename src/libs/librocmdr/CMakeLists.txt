include_directories( "inc" )

include_directories( ${DENSE_HASHMAP_INCLUDE_DIR} )
include_directories( ${LIBCPPTHREAD_INCLUDE_DIR} )

file( GLOB_RECURSE SRC_LIST "*.cpp" "src/" )

add_library( librocmdr ${SRC_LIST} )

target_link_libraries( librocmdr )
SET_TARGET_PROPERTIES( librocmdr PROPERTIES OUTPUT_NAME rocmdr )
