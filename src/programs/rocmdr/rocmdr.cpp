#include <algorithm>

#include <stdlib.h>

#include <data/ColumnData.h>
#include <RocMdrAnalysis.h>

#include "parsing.h"
#include "cmdline.h"

/**
 * Computes the number of distinct values in each column,
 * and returns a vector them.
 *
 * @param factors A set of columns.
 *
 *Ê@return A vector containing the counts of the distinct values
 *Ê        of each column.
 */
std::vector<unsigned char>
computeColumnRanges(ColumnData<unsigned char> &factors)
{
	std::vector<unsigned char> columnRanges;

	columnRanges.push_back( 3 ); // For SNP
	for(unsigned int i = 0; i < factors.size( ); i++)
	{
		const std::vector< unsigned char > column = factors.getColumn( i );
		unsigned char maxElement = *std::max_element( column.begin( ), column.end( ) ) + 1;
		columnRanges.push_back( maxElement );
	}

	return columnRanges;
}

/**
 * Outputs a set of AUC values and their corresponding p-values.
 *
 * @param pValues List of a p-value corresponding to each AUC value.
 */
void printPValues(const std::vector<float> &pValues)
{
    printf( "SNP\tAUC\tP-value\n" );
    for(unsigned int i = 0; i < pValues.size( ); i++)
    {
        printf("%d\t%f\n", i, pValues[ i ] );
    }
}

/**
 * Computes a ROCAUC for each SNP together with the
 * environmental factors. And prints the AUC and
 * it's corresponding p-value for each SNP.
 */
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
	ColumnData<unsigned char> genotypes;
	readToColumnData( argsInfo.inputs[ 0 ], &genotypes );

	// Read environmental factors
	ColumnData<unsigned char> factors;
	readToColumnData( argsInfo.inputs[ 1 ], &factors );

	// Read phenotypes
	std::vector<bool> phenotypes;
	readPhenotypes( argsInfo.inputs[ 2 ], &phenotypes );
	PhenotypeMapping phenotypeMapping( phenotypes );

	// Compute AUC with each SNP
	std::vector<float> pValues;
	for(unsigned int i = 0; i < genotypes.size( ); i++)
	{
		factors.addColumn( genotypes.getColumn( i ) );

		RocMdrAnalysis mdrAnalyzer( factors, phenotypeMapping );
		pValues.push_back( mdrAnalyzer.getPValue( ) );

		factors.removeColumnLast( );
	}

	// Print P-values
    printPValues( pValues );

    cmdline_parser_free( &argsInfo );

    return 0;
}
