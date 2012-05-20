#include <algorithm>
#include <set>
#include <vector>

#include <stdlib.h>

#include <data/ColumnData.h>
#include <Config.h>
#include <RocMdrAnalysis.h>
#include <RocMdrResult.h>
#include <RocMdrBatch.h>
#include <RocMdrPairBatch.h>
#include <RocMdrRestrictedBatch.h>

#include "RestrictReader.h"
#include "PairReader.h"
#include "plink/PlinkIo.h"
#include "rocmdr_snp_ggo.h"

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
 * Converts a list of locus names, to a corresponding list
 * of locus indices, and returns it.
 *
 * @param pairs List of pairs of locus names.
 * @param plinkIo The plink file that contains all loci.
 *
 * @return the corresponding list of locus indices.
 */
std::vector<std::pair<unsigned int, unsigned int> >
pairsToIndices(const std::vector< std::pair<std::string, std::string> > &pairs, PlinkIo &plinkIo)
{
	std::vector< std::pair<unsigned int, unsigned int> > mappedPairs;
	std::map<std::string, unsigned int> locusMap = plinkIo.getLocusMap( );
	for(unsigned int i = 0; i < pairs.size( ); i++)
	{
		std::pair<std::string, std::string> locusPair = pairs[ i ];

		if( locusMap.count( locusPair.first ) <= 0 &&
			locusMap.count( locusPair.second ) <= 0 )
		{
			continue;
		}

		unsigned int locusIndex1 = locusMap[ locusPair.first ];
		unsigned int locusIndex2 = locusMap[ locusPair.second ];

		mappedPairs.push_back( std::pair<unsigned int, unsigned int>( locusIndex1, locusIndex2 ) );
	}

	return mappedPairs;
}

/**
 * Reads the pairs from the given file, and return their
 * corresponding indices in the plink file.
 *
 * @param path Path to the pair file.
 * @param plinkIo The plink file that contains all loci.
 *
 * @return A list of pairs of indices.
 */
std::vector< std::pair<unsigned int, unsigned int> >
readPairIndices(const char *path, PlinkIo &plinkIo)
{
	PairReader reader( path );
	return pairsToIndices( reader.readSet( ), plinkIo );
}

/**
 * Convenience function that reads the restrict loci
 * from a file, converts them to indices and returns
 * the list of indices.
 *
 * @param restrictPath Path to the restrict file.
 * @param plinkIo The plink file that contains all loci.
 *
 * @return A list of indices corresponding to the given loci names.
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

	Config::getConfig( )->setNumThreads( argsInfo.num_threads_arg );
	Config::getConfig( )->setNumSimulations( argsInfo.num_simulations_arg );

	// Read SNPs
	PlinkIo plinkIo( argsInfo.inputs[ 0 ] );
	ColumnData<unsigned char> snps = plinkIo.getSnps( );
	PhenotypeMapping phenotypes( plinkIo.getPhenotypes( ) );

	RocMdrBatch *batch = NULL;
	if( argsInfo.restrict_file_given )
	{
		std::vector<unsigned int> restrictIndices = readRestrictIndices( argsInfo.restrict_file_arg, plinkIo );
		if( restrictIndices.size( ) == 0 )
		{
			printf( "rocmdr: no restrict set found in %s\n", argsInfo.restrict_file_arg );
			exit( EXIT_FAILURE );
		}

		batch = new RocMdrRestrictedBatch( snps, phenotypes, restrictIndices );
	}
	else if( argsInfo.pair_file_given )
	{
		std::vector< std::pair<unsigned int, unsigned int> > pairIndices =
				readPairIndices( argsInfo.pair_file_arg, plinkIo );
		if( pairIndices.size( ) == 0 )
		{
			printf( "rocmdr: no pairs found in %s\n", argsInfo.pair_file_arg );
			exit( EXIT_FAILURE );
		}

		batch = new RocMdrPairBatch( snps, phenotypes, pairIndices );
	}
	else
	{
		batch = new RocMdrBatch( snps, phenotypes );
	}

	std::vector<RocMdrResult> results;
	results = batch->run( argsInfo.interaction_order_arg );

    printPValues( plinkIo, results );

    delete batch;
    cmdline_parser_free( &argsInfo );

    return 0;
}
