find_package( CxxTest )

include_directories( ${LIBROCMDR_INCLUDE_DIR} )
include_directories( ${DENSE_HASHMAP_INCLUDE_DIR} )
include_directories( ${LIBCSV_INCLUDE_DIR} )

file( GLOB TEST_LIST "*.h" )

if( CXXTEST_FOUND )
    include_directories( ${CXXTEST_INCLUDE_DIR} )

    foreach( TEST_PATH ${TEST_LIST} )
        get_filename_component( TEST_NAME ${TEST_PATH} NAME_WE )

        cxxtest_add_test( ${TEST_NAME} "${TEST_NAME}.cpp" ${TEST_PATH} )
        target_link_libraries( ${TEST_NAME} librocmdr libcxxtest libcsv libcppthread )
    endforeach( TEST_PATH )
endif( )
