/*
 * RocMdrBatch.cpp
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */
#include <NullFilter.h>
#include <RocMdrAnalysis.h>

#include <bind.hpp>
#include <thread.hpp>
#include <threadfactory.hpp>

#include <RocMdrBatch.h>

RocMdrBatch::RocMdrBatch()
: m_numThreads( 1 )
{

}

void
RocMdrBatch::setNumThreads(unsigned int numThreads)
{
	m_numThreads = numThreads;
}

unsigned int
RocMdrBatch::getNumThreads()
{
	return m_numThreads;
}

void
runSingle(unsigned int start,
		  unsigned int end,
		  RecursionState state,
		  Filter *filter,
		  PhenotypeMapping &phenotypes,
		  std::vector<RocMdrResult> *results)
{
	NullInteraction nullSimulator( 100 );

	for(unsigned int i = start; i < end; i++)
	{
		if( filter->skip( i ) )
		{
			continue;
		}

		state.push( i );

		RocMdrAnalysis rocMdr( state.getCurrentSnps( ), phenotypes );

		float auc = rocMdr.getAuc( );
		float pValue = nullSimulator.getPValue( auc, state.getCurrentSnps( ), phenotypes );
		RocMdrResult result( state.getCurrentIndices( ), auc, pValue );

		results->push_back( result );

		state.pop( );
	}
}

void
runParallell(unsigned int start,
			 unsigned int end,
			 RecursionState &state,
			 Filter *filter,
		   	 PhenotypeMapping &phenotypes,
		   	 std::vector<RocMdrResult> *results,
		   	 unsigned int numThreads)
{
	unsigned int snpsPerThread = ( end - start + 1 ) / numThreads;

	std::vector< std::vector<RocMdrResult> > threadResults( numThreads );
	std::vector< CppThread::Thread * > threads;

	/**
	 * Split up work.
	 */
	for(unsigned int i = 0; i < numThreads; i++)
	{
		unsigned int threadStart = start + i * snpsPerThread;
		unsigned int threadEnd = threadStart + snpsPerThread;
		if( i == numThreads - 1 )
		{
			threadEnd = end;
		}

		CppThread::Thread *thread = CppThread::ThreadFactory::getInstance( )->createThread( );
		CppThread::Functor *f = new CppThread::Functorf6<
												unsigned int,
												unsigned int,
												RecursionState,
												Filter *,
												PhenotypeMapping &,
												std::vector<RocMdrResult> *>( runSingle,
																			  threadStart,
																			  threadEnd,
																			  state,
																			  filter,
																			  phenotypes,
																			  &threadResults[ i ] );
		thread->start( f );
		threads.push_back( thread );
	}

	/**
	 * Gather results.
	 */
	for(unsigned int i = 0; i < numThreads; i++)
	{
		threads[ i ]->join( );

		results->insert( results->end( ),
						 threadResults[ i ].begin( ),
						 threadResults[ i ].end( ) );
	}

}

void
RocMdrBatch::runRocMdrRecursive(RecursionState &state,
		   	   	   	   	   	    ColumnData<unsigned char> &snps,
		   	   	   	   	   	    PhenotypeMapping &phenotypes,
		   	   	   	   	   	    std::vector<RocMdrResult> *results)
{
	if( state.done( ) )
	{
		NullFilter filter;
		if( getNumThreads( ) <= 1 || snps.size( ) < getNumThreads( ) )
		{
			runSingle( state.nextIndex( ), snps.size( ), state, &filter, phenotypes, results );
		}
		else
		{
			runParallell( state.nextIndex( ), snps.size( ), state, &filter, phenotypes, results, getNumThreads( ) );
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

	RecursionState state( interactionOrder, snps );
	runRocMdrRecursive( state, snps, phenotypes, &results );

	return results;
}
