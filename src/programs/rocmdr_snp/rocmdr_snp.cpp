#include <algorithm>
#include <set>
#include <vector>

#include <stdlib.h>

#include <data/ColumnData.h>
#include <RocMdrAnalysis.h>
#include <RocMdrResult.h>
#include <RocMdrBatch.h>
#include <RocMdrRestrictedBatch.h>

#include "plink/PlinkIo.h"
#include "cmdline.h"

/**
 * Converts the names in the restrict set to a list of
 * indices in the loci array.
 *
 * @param restrictSet Set of loci to be converted to indices.
 * @param plinkIo The plink file that contains all loci.
 *
 * @return A list of indices corresponding to the given loci names.
 */
std::vector<unsigned int>
namesToIndices(const std::set<std::string> &restrictSet, PlinkIo &plinkIo)
{
	std::vector<unsigned int> lociAsIndices;
	unsigned int numLoci = plinkIo.getSnps( ).size( );
	for(unsigned int i = 0; i < numLoci; i++)
	{
		if( restrictSet.count( plinkIo.getLocus( i ) ) > 0 )
		{
			lociAsIndices.push_back( i );
		}
	}

	return lociAsIndices;
}

/**
 * Convenience function that reads the restrict loci
 * from a file, converts them to indices and returns
 * the list of indices.
 *
 * @param restrictPath Path to the restrict file.
 * @param plinkIo The plink file that contains all loci.
 *
 * @param A list of indices corresponding to the given loci names.
 */
std::vector<unsigned int>
readRestrictIndices(const char *restrictPath, PlinkIo &plinkIo)
{
	RestrictReader reader( restrictPath );
	return namesToIndices( reader.readSet( ), plinkIo );
}

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
		exit( EXIT_FAILURE );
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

	std::vector<RocMdrResult> results;
	if( !argsInfo.restrict_file_given )
	{
		RocMdrBatch batch;
		results = batch.run( argsInfo.interaction_order_arg, snps, phenotypes );
	}
	else
	{
		std::vector<unsigned int> restrictIndices = readRestrictIndices( argsInfo.restrict_file_arg, plinkIo );
		if( restrictIndices.size( ) == 0 )
		{
			printf( "rocmdr: no restrict set found in %s\n", argsInfo.restrict_file_arg );
			exit( EXIT_FAILURE );
		}

		RocMdrRestrictedBatch batch( argsInfo.interaction_order_arg );
		results = batch.run( restrictIndices, snps, phenotypes );
	}

    printPValues( plinkIo, results );

    cmdline_parser_free( &argsInfo );

    return 0;
}
