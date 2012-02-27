#include <algorithm>

#include <stdlib.h>

#include <data/ColumnData.h>
#include <RocMdrAnalysis.h>
#include <RocMdrResult.h>
#include <RocMdrBatch.h>

#include "plink/PlinkIo.h"
#include "cmdline.h"

/**
 * Outputs the AUC and P-value for each result, and the loci
 * that was involved in the result.
 *
 * @param results List of results form a RocMdr analysis.
 */
void printPValues(PlinkIo &plinkIo, const std::vector<RocMdrResult> &results)
{
	printf( "Loci\tAUC\tP-value\n" );

	for(unsigned int i = 0; i < results.size( ); i++)
	{
		const RocMdrResult &result = results[ i ];
		const std::vector<unsigned int> &loci = result.getLoci( );
		for(unsigned int k = 0; k < loci.size( ); k++)
		{
			printf( "%s ", plinkIo.getLocus( loci[ k ] ).c_str( ) );
		}
		printf( "\t%f\t%E\n", result.getAuc( ), result.getPValue( ) );
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

	if( argsInfo.inputs_num != 1 )
	{
		printf( "rocmdr: wrong number of arguments.\n" );
		cmdline_parser_print_help( );
		exit( EXIT_FAILURE );
	}

	// Read SNPs
	PlinkIo plinkIo( argsInfo.inputs[ 0 ] );
	ColumnData<unsigned char> snps = plinkIo.getSnps( );
	PhenotypeMapping phenotypes( plinkIo.getPhenotypes( ) );

	RocMdrBatch batch;
	std::vector<RocMdrResult> results = batch.run( argsInfo.interaction_order_arg, snps, phenotypes );

    printPValues( plinkIo, results );

    cmdline_parser_free( &argsInfo );

    return 0;
}
