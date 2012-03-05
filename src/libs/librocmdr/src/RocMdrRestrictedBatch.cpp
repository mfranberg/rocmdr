/*
 * RocMdrBatch.cpp
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */
#include <RocMdrAnalysis.h>

#include <RocMdrRestrictedBatch.h>

RocMdrRestrictedBatch::RocMdrRestrictedBatch(unsigned int order)
: m_order( order )
{
	m_order = order;
}

void
RocMdrRestrictedBatch::runRocMdrRecursive(RecursionState &state,
		   	   	   	   	   	   	   	   	  std::vector<unsigned int> &restricted,
		   	   	   	   	   	   	   	   	  ColumnData<unsigned char> &snps,
		   	   	   	   	   	   	   	   	  PhenotypeMapping &phenotypes,
		   	   	   	   	   	   	   	   	  std::vector<RocMdrResult> *results)
{
	if( state.done( ) )
	{
		std::set<unsigned int> restrictedSet( restricted.begin( ), restricted.end( ) );

		for(unsigned int i = 0; i < snps.size( ); i++)
		{
			if( restrictedSet.count( i ) > 0 )
			{
				continue;
			}

			state.push( i );

			RocMdrAnalysis rocMdr( state.getCurrentSnps( ), phenotypes );
			float auc = rocMdr.getAuc( );

			RocMdrResult result( state.getCurrentIndices( ), auc, m_nullSimulator.computePValue( auc ) );

			results->push_back( result );

			state.pop( );
		}
	}
	else
	{
		for(unsigned int i = state.nextIndex( ); i < restricted.size( ); i++)
		{
			state.push( restricted[ i ], i );

			runRocMdrRecursive( state, restricted, snps, phenotypes, results );

			state.pop( );
		}
	}
}

std::vector<RocMdrResult>
RocMdrRestrictedBatch::run(std::vector<unsigned int> restricted,
		  	     ColumnData<unsigned char> snps,
		  	     PhenotypeMapping phenotypes)
{
	std::vector<RocMdrResult> results;

	m_nullSimulator.setData( snps, phenotypes.getPhenotypes( ) );
	m_nullSimulator.setOrder( m_order + 1 );
	m_nullSimulator.setNumberOfSamples( restricted.size( ) * snps.size( ) );

	RecursionState state( m_order, snps );
	runRocMdrRecursive( state, restricted, snps, phenotypes, &results );

	return results;
}
