/*
 * RocMdrPairBatch.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: fmattias
 */
#include <Config.h>
#include <RocMdrAnalysis.h>
#include <data/ColumnData.h>
#include <data/PhenotypeMapping.h>

#include <bind.hpp>
#include <thread.hpp>
#include <threadfactory.hpp>

#include <RocMdrPairBatch.h>

RocMdrPairBatch::RocMdrPairBatch(ColumnData<unsigned char> snps,
								 PhenotypeMapping phenotypes,
								 const std::vector<std::pair<unsigned int, unsigned int> > &pairs)
: RocMdrBatch( snps, phenotypes ),
  m_pairs( pairs )
{

}

void
RocMdrPairBatch::runSingle(unsigned int start, unsigned int end, std::vector<RocMdrResult> *results)
{
	NullInteraction nullSimulator( Config::getConfig( )->getNumSimulations( ) );

	for(unsigned int i = start; i < end; i++)
	{
		std::pair<unsigned int, unsigned int> locusPair = m_pairs[ i ];

		ColumnData<unsigned char> snps;
		snps.addColumn( getSnps( ).getColumn( locusPair.first ) );
		snps.addColumn( getSnps( ).getColumn( locusPair.second ) );

		RocMdrAnalysis rocMdr( snps, getPhenotypes( ) );

		float auc = rocMdr.getAuc( );
		float pValue = nullSimulator.getPValue( auc, snps, getPhenotypes( ) );

		std::vector<unsigned int> curIndicies;
		curIndicies.push_back( locusPair.first );
		curIndicies.push_back( locusPair.second );
		RocMdrResult result( curIndicies, auc, pValue );

		results->push_back( result );
	}
}

void
RocMdrPairBatch::runParallell(unsigned int start, unsigned int end, std::vector<RocMdrResult> *results)
{
	unsigned int numThreads = Config::getConfig( )->getNumThreads( );
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
		CppThread::Functor *f = CppThread::bind( &RocMdrPairBatch::runSingle,
												 this,
												 threadStart,
												 threadEnd,
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

std::vector<RocMdrResult>
RocMdrPairBatch::run(unsigned int interactionOrder)
{
	std::vector<RocMdrResult> results;
	if( Config::getConfig( )->getNumThreads( ) == 1 )
	{
		runSingle( 0, m_pairs.size( ), &results );
	}
	else
	{
		runParallell( 0, m_pairs.size( ), &results );
	}

	return results;
}
