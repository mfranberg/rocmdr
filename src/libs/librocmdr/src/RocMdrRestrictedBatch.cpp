/*
 * RocMdrBatch.cpp
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */
#include <RestrictFilter.h>
#include <RocMdrBatch.h>
#include <RocMdrAnalysis.h>

#include <RocMdrRestrictedBatch.h>

RocMdrRestrictedBatch::RocMdrRestrictedBatch(const std::vector<unsigned int> &restricted)
: RocMdrBatch::RocMdrBatch( ),
  m_restricted( restricted )
{

}

void
RocMdrRestrictedBatch::runRocMdrRecursive(RecursionState &state,
		   	   	   	   	   	   	   	   	  ColumnData<unsigned char> &snps,
		   	   	   	   	   	   	   	   	  PhenotypeMapping &phenotypes,
		   	   	   	   	   	   	   	   	  std::vector<RocMdrResult> *results)
{
	if( state.done( ) )
	{
		RestrictFilter filter( m_restricted );
		if( getNumThreads( ) <= 1 || snps.size( ) < getNumThreads( ) )
		{
			runSingle( 0, snps.size( ), state, &filter, phenotypes, results );
		}
		else
		{
			runParallell( 0, snps.size( ), state, &filter, phenotypes, results, getNumThreads( ) );
		}
	}
	else
	{
		for(unsigned int i = state.nextIndex( ); i < m_restricted.size( ); i++)
		{
			state.push( m_restricted[ i ], i );

			runRocMdrRecursive( state, snps, phenotypes, results );

			state.pop( );
		}
	}
}

std::vector<RocMdrResult>
RocMdrRestrictedBatch::run(unsigned int interactionOrder,
		  	     ColumnData<unsigned char> snps,
		  	     PhenotypeMapping phenotypes)
{
	std::vector<RocMdrResult> results;

	RecursionState state( interactionOrder + 1, snps );

	runRocMdrRecursive( state, snps, phenotypes, &results );

	return results;
}
