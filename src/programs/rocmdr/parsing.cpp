/*
 * parsing.cpp
 *
 *  Created on: Sep 26, 2011
 *      Author: fmattias
 */

#include <vector>
#include <CSVReader.h>
#include <CSVIntProcessor.h>

#include "parsing.h"

void
readToColumnData(const char *path, ColumnData<unsigned int> *data)
{
	CSVReader parser;
	CSVIntProcessor processor;
	if( !parser.parse( path, &processor ) )
	{
		printf( "rocmdr: No such file %s\n", path );
		exit( 0 );
	}

	std::vector< std::vector< int > > columns = processor.getColumnData( );
	for(unsigned int i = 0; i < columns.size( ); i++)
	{
		std::vector<int> currentColumn = columns[ i ];
		std::vector<unsigned int> unsignedColumn( currentColumn.begin( ), currentColumn.end( ) );
		data->addColumn( unsignedColumn );
	}
}

void
readPhenotypes(const char *path, std::vector<bool> *phenotypes)
{
	CSVReader parser;
	CSVIntProcessor processor;
	if( !parser.parse( path, &processor ) )
	{
		printf( "rocmdr: No such file %s\n", path );
		exit( 0 );
	}

	std::vector< int > phenotypesAsInt = processor.getColumnData( )[ 0 ];
	for(unsigned int i = 0; i < phenotypesAsInt.size( ); i++)
	{
		phenotypes->push_back( phenotypesAsInt[ i ] == 1 );
	}
}
