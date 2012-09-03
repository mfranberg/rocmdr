/*
 * InteractionIterator.h
 *
 *  Created on: Aug 29, 2012
 *      Author: fmattias
 */

#ifndef INTERACTIONITERATOR_H_
#define INTERACTIONITERATOR_H_

#include <vector>

#include <data/ColumnData.h>
#include <data/PhenotypeMapping.h>

/**
 * Iterates over pairs of indices belonging to
 * interactions.
 */
class InteractionIterator
{
public:
	/**
	 * Interface for the visitor.
	 */
	class InteractionVisitor
	{
	public:
		/**
		 * Called for each interaction pair.
		 *
		 * @param snps The data for the snps.
		 * @param indices The indices for each snp.
		 */
		virtual void visit(ColumnData<unsigned char> &snps, std::vector<unsigned int> &indices, PhenotypeMapping &phenotypes) = 0;
	};

	/**
	 * Visits all interaction pairs for this iterator type
	 * and notifies the visitor for each pair.
	 *
	 * @param visitor The visitor for each pair.
	 */
	virtual void visitAll(InteractionVisitor *visitor) = 0;

	/**
	 * Creates a new iterator that iterates over a subset
	 * of this iterator.
	 *
	 * This is used to divide an iterator to several threads.
	 *
	 * @param start Start index of the subset iterator.
	 * @param end End index of the subset iterator.
	 *
	 * @return The subset iterator, must be deallocated by the caller.
	 */
	virtual InteractionIterator * partial(size_t index, size_t num_parts) = 0;
};

#endif /* INTERACTIONITERATOR_H_ */
