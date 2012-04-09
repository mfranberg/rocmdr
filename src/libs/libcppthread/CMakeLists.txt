include_directories( "." )

add_library( libcppthread thread.cpp threadfactory.cpp pthread.cpp pthreadfactory.cpp )

SET_TARGET_PROPERTIES( libcppthread PROPERTIES OUTPUT_NAME cppthread )
