/*
 * RocMdrBatch.h
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */

#ifndef ROCMDRBATCH_H_
#define ROCMDRBATCH_H_

#include <vector>

#include <data/ColumnData.h>
#include <iter/InteractionIterator.h>
#include <RecursionState.h>

/**
 * Represents a set of computations that will be
 * performed on different subsets of a set of SNPs.
 * This batch will do all computations and return
 * a list of results, where each result is bound
 * to a subset of the snps.
 */
class AllIterator
: public InteractionIterator
{
public:
	/**
	 * Constructor.
	 *
	 * @param snps The snps.
	 * @param phenotypes The phenotypes.
	 * @param interactionOrder The number of snps considered jointly.
	 */
	AllIterator(ColumnData<unsigned char> &snps, PhenotypeMapping &phenotypes, unsigned int interactionOrder);

	/**
	 * Constructor for subset iterator.
	 *
	 * @param other Iterator to base the subset iterator on.
	 * @param index Index of this partial iterator.
	 * @param numParts Number of partial iterators.
	 */
	AllIterator(const AllIterator &other, size_t index, size_t numParts);

	/**
	 * Copy constructor.
	 */
	AllIterator(const AllIterator &other);

	/**
	 * Assignment operator.
	 */
	AllIterator & operator=(const AllIterator &other);

	/**
	 * Destructor.
	 */
	virtual ~AllIterator();

	/**
	 * @see InteractionIterator::visitAll.
	 */
	void visitAll(InteractionVisitor *visitor);

	/**
	 * Visits all interaction pairs recursively.
	 *
	 * @param visitor The visitor.
	 * @param state Holds the state for the recursion.
	 */
	void visitAllRecursive(InteractionVisitor *visitor, RecursionState &state);

	/**
	 * @see InteractionIterator::part.
	 */
	InteractionIterator * partial(size_t index, size_t numParts);

private:

	/**
	 * SNPs.
	 */
	ColumnData<unsigned char> &m_snps;

	/**
	 * Phenotypes.
	 */
	PhenotypeMapping &m_phenotypes;

	/**
	 * Index of this partial iterator.
	 */
	size_t m_index;

	/**
	 * Number of partial iterators.
	 */
	size_t m_numParts;

	/**
	 * The number of snps considered jointly.
	 */
	unsigned int m_interactionOrder;
};

#endif /* ROCMDRBATCH_H_ */
