include_directories( "." )

file( GLOB SRC_LIST "cxxtest/*.cpp" )

add_library( libcxxtest ${SRC_LIST} )

SET_TARGET_PROPERTIES( libcxxtest PROPERTIES OUTPUT_NAME cxxtest )
