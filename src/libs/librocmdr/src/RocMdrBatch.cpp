/*
 * RocMdrBatch.cpp
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */
#include <RocMdrAnalysis.h>

#include <RocMdrBatch.h>

void
RocMdrBatch::runRocMdrRecursive(RecursionState &state,
		   	   	   	   	   	    ColumnData<unsigned char> &snps,
		   	   	   	   	   	    PhenotypeMapping &phenotypes,
		   	   	   	   	   	    std::vector<RocMdrResult> *results)
{
	if( state.done( ) )
	{
		for(unsigned int i = state.nextIndex( ); i < snps.size( ); i++)
		{
			state.push( i );

			RocMdrAnalysis rocMdr( state.getCurrentSnps( ), phenotypes );
			float auc = rocMdr.getAuc( );
			RocMdrResult result( state.getCurrentIndices( ),
								 auc,
								 m_nullSimulator.computePValue( auc ) );

			results->push_back( result );

			state.pop( );
		}
	}
	else
	{
		for(unsigned int i = state.nextIndex( ); i < snps.size( ); i++)
		{
			state.push( i );

			runRocMdrRecursive( state, snps, phenotypes, results );

			state.pop( );
		}
	}
}

std::vector<RocMdrResult>
RocMdrBatch::run(unsigned int interactionOrder,
					  ColumnData<unsigned char> snps,
					  PhenotypeMapping phenotypes)
{
	std::vector<RocMdrResult> results;

	m_nullSimulator.setData( snps, phenotypes.getPhenotypes( ) );
	m_nullSimulator.setOrder( interactionOrder );

	RecursionState state( interactionOrder - 1, snps );
	runRocMdrRecursive( state, snps, phenotypes, &results );

	return results;
}
