/*
 * CSVReader.cpp
 *
 *  Created on: Sep 19, 2011
 *      Author: fmattias
 */
#include <iostream>

#include "CSVProcessor.h"
#include "CSVReader.h"

/**
 * Callback function for the C CSV parser, takes
 * a field and passes it on to the assigned CSVProcessor.
 *
 * @param field Pointer to a string.
 * @param length Length of the field.
 * @param data Pointer to a CSVProcessor.
 */
static void
newField(void *field, unsigned long int length, void *data)
{
	std::string stringField( (char *) field, length );
	CSVProcessor *manager = (CSVProcessor *) data;
	manager->onField( stringField );
}

/**
 * Callback function for the C CSV parser, notifies
 * the assigned CSVProcessor that a row has ended.
 *
 * @param field Pointer to a string.
 * @param length Length of the field.
 * @param data Pointer to a CSVProcessor.
 */
static void
newRow(int number, void *data)
{
	CSVProcessor *manager = (CSVProcessor *) data;
	manager->onRowEnd( );
}

CSVReader::CSVReader(char delimiter)
: m_delimiter( delimiter ),
  m_currentProcessor( 0 )
{
}

bool
CSVReader::parse(const std::string path, CSVProcessor *processor)
{
	FILE *inputFile = fopen( path.c_str( ), "r" );
	if( inputFile == NULL )
	{
		return false;
	}

	bool parseStatus = parse( inputFile, processor );
	fclose( inputFile );

	return parseStatus;
}

bool
CSVReader::parse(FILE *input, CSVProcessor *processor)
{
	m_currentProcessor = processor;

	struct csv_parser csvParser;
	csv_init( &csvParser, 0 );
	csv_set_delim( &csvParser, m_delimiter );

	char readBuffer[ CSV_BUFFER_SIZE + 1 ];
	while( !feof( input ) )
	{
		int numBytesRead = fread( &readBuffer[ 0 ], sizeof( char ), CSV_BUFFER_SIZE, input );
		csv_parse( &csvParser, readBuffer, numBytesRead, &newField, &newRow, (void *) processor );
	}

	csv_fini( &csvParser, &newField, &newRow, (void *) processor );
	csv_free( &csvParser );

	return true;
}

CSVProcessor *
CSVReader::getCurrentProcessor()
{
	return m_currentProcessor;
}
