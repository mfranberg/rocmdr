#include <algorithm>

#include <stdlib.h>

#include <ColumnData.h>
#include <RocMdrAnalysis.h>

#include "auc/NoAssociationSimulator.h"

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
std::vector<unsigned int>
computeColumnRanges(ColumnData<unsigned int> &factors)
{
	std::vector<unsigned int> columnRanges;

	columnRanges.push_back( 3 ); // For SNP
	for(unsigned int i = 0; i < factors.size( ); i++)
	{
		const std::vector< unsigned int > column = factors.getColumn( i );
		unsigned int maxElement = *std::max_element( column.begin( ), column.end( ) ) + 1;
		columnRanges.push_back( maxElement );
	}

	return columnRanges;
}

/**
 * Computes the p-values for a given vector of AUC values.
 *
 * @param factors A set of columns.
 * @param phenotypes A list of case/control classifications.
 * @param aucValues A list of AUC values to compute p-values for.
 *
 * @return A list of p-values corresponding to each AUC value.
 */
std::vector<float>
computePValues(ColumnData<unsigned int> &factors, const PhenotypeMapping &phenotypes, const std::vector<float> &aucValues)
{
	NoAssociationSimulator pValueSimulator( factors, phenotypes );

	std::vector<float> pValues( aucValues.size( ), 0.0f );
	for(unsigned int i = 0; i < aucValues.size( ); i++)
	{
		pValues[ i ] = pValueSimulator.computePValue( aucValues[ i ] );
	}

	return pValues;
}

/**
 * Outputs a set of AUC values and their corresponding p-values.
 *
 * @param aucValues List of AUC values.
 * @param pValues List of a p-value corresponding to each AUC value.
 *
 */
void printAUCValues(const std::vector<float> &aucValues, const std::vector<float> &pValues)
{
    printf( "SNP\tAUC\tP-value\n" );
    for(unsigned int i = 0; i < aucValues.size( ); i++)
    {
        printf("%d\t%f\t%f\n", i, aucValues[ i ], pValues[ i ] );
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
	ColumnData<unsigned int> genotypes;
	readToColumnData( argsInfo.inputs[ 0 ], &genotypes );

	// Read environmental factors
	ColumnData<unsigned int> factors;
	readToColumnData( argsInfo.inputs[ 1 ], &factors );

	// Read phenotypes
	std::vector<bool> phenotypes;
	readPhenotypes( argsInfo.inputs[ 2 ], &phenotypes );
	PhenotypeMapping phenotypeMapping( phenotypes );

	// Compute AUC with each SNP
	std::vector<float> aucValues;
	for(unsigned int i = 0; i < genotypes.size( ); i++)
	{
		factors.addColumn( genotypes.getColumn( i ) );

		RocMdrAnalysis mdrAnalyzer( factors, phenotypeMapping );
		aucValues.push_back( mdrAnalyzer.calculateAuc( ) );

		factors.removeColumnLast( );
	}

	// Print AUCs
	std::vector<unsigned int> columnRanges = computeColumnRanges( factors );
	std::vector<float> pValues = computePValues( factors, phenotypeMapping, aucValues );
    printAUCValues( aucValues, pValues );

    cmdline_parser_free( &argsInfo );

    return 0;
}
