#include <stdlib.h>

#include <ColumnData.h>

#include "CSVReader.h"
#include "CSVIntProcessor.h"

#include "cmdline.h"

void readToColumnData(const char *path, ColumnData<int> *data)
{
	CSVReader parser;
	CSVIntProcessor processor;
	parser.parse( path, &processor );

	std::vector< std::vector< int > > columns = processor.getColumnData( );
	for(unsigned int i = 0; i < columns.size( ); i++)
	{
		data->addColumn( columns[ i ] );
	}
}

int main(int argc, char **argv)
{
	gengetopt_args_info argsInfo;
	if( cmdline_parser( argc, argv, &argsInfo ) != 0 )
	{
		exit( -1 );
	}

	if( argsInfo.inputs_num != 3 )
	{
		printf( "rocmdr: wrong number of arguments.\n" );
		cmdline_parser_print_help( );
		exit( EXIT_FAILURE );
	}

	std::string snpFile = argsInfo.inputs[ 0 ];
	std::string factorFile = argsInfo.inputs[ 1 ];
	std::string phenotypeFile = argsInfo.inputs[ 2 ];

	// Read SNPs
	ColumnData<int> genotypes;
	readToColumnData( argsInfo.inputs[ 0 ], &genotypes );

	// Read environmental factors
	ColumnData<int> factors;
	readToColumnData( argsInfo.inputs[ 1 ], &factors );

	// Read phenotypes
	ColumnData<int> phenotypes;
	readToColumnData( argsInfo.inputs[ 2 ], &phenotypes );

	cmdline_parser_free( &argsInfo );

    return 0;
}
