/*
 * InteractionBatch.h
 *
 *  Created on: Sep 3, 2012
 *      Author: fmattias
 */

#ifndef INTERACTIONBATCH_H_
#define INTERACTIONBATCH_H_

#include <vector>

#include <iter/InteractionIterator.h>
#include <RocMdrVisitor.h>
#include <RocMdrResult.h>

/**
 * This class is responsible for running a RocMdrAnalysis
 * on a given set of interactions.
 */
class InteractionBatch
{
public:
	/**
	 * Internal function that is run on each thread.
	 *
	 * @param iter The iterator to use for finding interactions.
	 * @param visitor The visitor that will process the interactions.
	 */
	void run(InteractionIterator *iter, RocMdrVisitor *visitor);

	/**
	 * Splits the given iterator into a given number
	 * of partial iterators, so that they may run on
	 * several threads. The set of  interactions each
	 * traverses is disjoint.
	 *
	 * @param iter The iterator to split into partial iterators.
	 * @param numThreads The number of partial iterators.
	 *
	 * @return A list of partial iterators, that together traverse
	 *         the same interactions as the input iterator.
	 */
	std::vector<InteractionIterator *> split(InteractionIterator *iter, unsigned int numThreads);

	/**
	 * Applies RocMdr to each interaction given by the iterator,
	 * and returns their results.
	 *
	 * @param iter An iterator over interactions.
	 * @param numThreads The number of threads to use.
	 *
	 * @return A list of RocMdrResults.
	 */
	std::vector<RocMdrResult> run(InteractionIterator *iter, unsigned int numThreads);
};



#endif /* INTERACTIONBATCH_H_ */
