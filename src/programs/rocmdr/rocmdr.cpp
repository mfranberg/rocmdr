#include <stdlib.h>

#include <ColumnData.h>
#include <RocMdrAnalysis.h>

#include "CSVReader.h"
#include "CSVIntProcessor.h"

#include "cmdline.h"

void readToColumnData(const char *path, ColumnData<unsigned int> *data)
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

void readPhenotypes(const char *path, std::vector<bool> *phenotypes)
{
	CSVReader parser;
	CSVIntProcessor processor;
	parser.parse( path, &processor );

	std::vector< int > phenotypesAsInt = processor.getColumnData( )[ 0 ];
	for(unsigned int i = 0; i < phenotypesAsInt.size( ); i++)
	{
		phenotypes->push_back( phenotypesAsInt[ i ] == 1 );
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

	// Read SNPs
	ColumnData<unsigned int> genotypes;
	readToColumnData( argsInfo.inputs[ 0 ], &genotypes );

	// Read environmental factors
	ColumnData<unsigned int> factors;
	readToColumnData( argsInfo.inputs[ 1 ], &factors );

	// Read phenotypes
	std::vector<bool> phenotypes;
	readPhenotypes( argsInfo.inputs[ 2 ], &phenotypes );

	// Compute original AUC
	PhenotypeMapping phenotypeMapping( phenotypes );
	RocMdrAnalysis mdrAnalyzer( factors, phenotypeMapping );
	float originalAUC = mdrAnalyzer.calculateAuc( );

	// Compute AUC with each SNP
	std::vector<float> aucValues;
	for(unsigned int i = 0; i < genotypes.size( ); i++)
	{
		factors.addColumn( genotypes.getColumn( i ) );

		RocMdrAnalysis mdrAnalyzer( factors, phenotypeMapping );
		aucValues.push_back( mdrAnalyzer.calculateAuc( ) );

		factors.removeColumnLast( );
	}

	// Print AUC ratios
	printf("SNP\tAUC\tratio\n");
	for( unsigned int i = 0; i < aucValues.size( ); i++)
	{
		printf( "%d\t%f\t%f\n", i, aucValues[ i ], aucValues[ i ] / originalAUC );
	}

	cmdline_parser_free( &argsInfo );

    return 0;
}
