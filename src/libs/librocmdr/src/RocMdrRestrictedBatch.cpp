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

RocMdrRestrictedBatch::RocMdrRestrictedBatch(ColumnData<unsigned char> snps, PhenotypeMapping phenotypes, const std::vector<unsigned int> &restricted)
: RocMdrBatch::RocMdrBatch( snps, phenotypes ),
  m_restricted( restricted )
{

}

void
RocMdrRestrictedBatch::runRocMdrRecursive(RecursionState &state,
		   	   	   	   	   	   	   	   	  std::vector<RocMdrResult> *results)
{
	if( state.done( ) )
	{
		if( getNumThreads( ) <= 1 || getSnps( ).size( ) < getNumThreads( ) )
		{
			runSingle( 0, getSnps( ).size( ), state, results );
		}
		else
		{
			runParallell( 0, getSnps( ).size( ), state, results );
		}
	}
	else
	{
		for(unsigned int i = state.nextIndex( ); i < m_restricted.size( ); i++)
		{
			state.push( m_restricted[ i ], i );

			runRocMdrRecursive( state, results );

			state.pop( );
		}
	}
}

std::vector<RocMdrResult>
RocMdrRestrictedBatch::run(unsigned int interactionOrder)
{
	std::vector<RocMdrResult> results;

	ColumnData<unsigned char> snps = getSnps( );
	RecursionState state( interactionOrder + 1, snps );

	setFilter( new RestrictFilter( m_restricted ) );
	runRocMdrRecursive( state, &results );

	return results;
}
