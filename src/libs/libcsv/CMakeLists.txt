include_directories( "inc/" )

add_library( libcsv src/libcsv.c src/CSVIntProcessor.cpp src/CSVReader.cpp )

SET_TARGET_PROPERTIES( libcsv PROPERTIES OUTPUT_NAME csv )
