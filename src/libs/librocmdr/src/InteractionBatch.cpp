/*
 * RocMdrBatch.cpp
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */
#include <RocMdrVisitor.h>

#include <bind.hpp>
#include <thread.hpp>
#include <threadfactory.hpp>

#include <InteractionBatch.h>

void
InteractionBatch::run(InteractionIterator *iter, RocMdrVisitor *visitor)
{
	iter->visitAll( visitor );
}

std::vector<InteractionIterator *>
InteractionBatch::split(InteractionIterator *iter, unsigned int numThreads)
{
	std::vector<InteractionIterator *> iterators;
	for(size_t i  = 0; i < numThreads; i++)
	{
		iterators.push_back( iter->partial( i, numThreads ) );
	}

	return iterators;
}

std::vector<RocMdrResult>
InteractionBatch::run(InteractionIterator *iter, unsigned int numThreads)
{
	std::vector<InteractionIterator *> iterators = split( iter, numThreads );
	std::vector<RocMdrVisitor> visitors( numThreads );
	std::vector< CppThread::Thread * > threads;

	/**
	 * Split up work.
	 */
	for(unsigned int i = 0; i < numThreads; i++)
	{


		CppThread::Thread *thread = CppThread::ThreadFactory::getInstance( )->createThread( );
		CppThread::Functor *f = CppThread::bind( &InteractionBatch::run,
												 this,
												 iterators[ i ],
												 &visitors[ i ]
												 );

		thread->start( f );
		threads.push_back( thread );
	}

	/**
	 * Gather results.
	 */
	std::vector<RocMdrResult> results;
	for(unsigned int i = 0; i < numThreads; i++)
	{
		threads[ i ]->join( );
		delete iterators[ i ];

		std::vector<RocMdrResult> threadResult = visitors[ i ].getResults( );

		results.insert( results.end( ), threadResult.begin( ), threadResult.end( ) );
	}

	return results;
}
