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

RocMdrBatch::RocMdrBatch(ColumnData<unsigned char> snps, PhenotypeMapping phenotypes)
:	m_snps( snps ),
 	m_phenotypes( phenotypes ),
 	m_filter( NULL ),
	m_numThreads( 1 )
{

}

RocMdrBatch::~RocMdrBatch()
{
	if( m_filter != NULL )
	{
		delete m_filter;
	}
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

ColumnData<unsigned char>
RocMdrBatch::getSnps()
{
	return m_snps;
}

void
RocMdrBatch::setFilter(Filter *filter)
{
	if( m_filter != NULL )
	{
		delete m_filter;
	}

	m_filter = filter;
}

void
RocMdrBatch::runSingle(unsigned int start,
		  	  	  	   unsigned int end,
		  	  	  	   RecursionState state,
		  	  	  	   std::vector<RocMdrResult> *results)
{
	NullInteraction nullSimulator( 100 );

	for(unsigned int i = start; i < end; i++)
	{
		if( m_filter != NULL && m_filter->skip( i ) )
		{
			continue;
		}

		state.push( i );

		RocMdrAnalysis rocMdr( state.getCurrentSnps( ), m_phenotypes );

		float auc = rocMdr.getAuc( );
		float pValue = nullSimulator.getPValue( auc, state.getCurrentSnps( ), m_phenotypes );
		RocMdrResult result( state.getCurrentIndices( ), auc, pValue );

		results->push_back( result );

		state.pop( );
	}
}

void
RocMdrBatch::runParallell(unsigned int start,
			 unsigned int end,
			 RecursionState &state,
		   	 std::vector<RocMdrResult> *results)
{
	unsigned int numThreads = getNumThreads( );
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
		CppThread::Functor *f = CppThread::bind( &RocMdrBatch::runSingle,
												 this,
												 threadStart,
												 threadEnd,
												 state,
												 &threadResults[ i ]
												 );

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
		   	   	   	   	   	    std::vector<RocMdrResult> *results)
{
	if( state.done( ) )
	{
		NullFilter filter;
		if( getNumThreads( ) <= 1 || m_snps.size( ) < getNumThreads( ) )
		{
			runSingle( state.nextIndex( ), m_snps.size( ), state, results );
		}
		else
		{
			runParallell( state.nextIndex( ), m_snps.size( ), state, results );
		}
	}
	else
	{
		for(unsigned int i = state.nextIndex( ); i < m_snps.size( ); i++)
		{
			state.push( i );

			runRocMdrRecursive( state, results );

			state.pop( );
		}
	}
}

std::vector<RocMdrResult>
RocMdrBatch::run(unsigned int interactionOrder)
{
	std::vector<RocMdrResult> results;

	RecursionState state( interactionOrder, m_snps );
	runRocMdrRecursive( state, &results );

	return results;
}
