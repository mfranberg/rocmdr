/*
 * RocMdrRestrictedBatch.h
 *
 *  Created on: Feb 27, 2012
 *      Author: fmattias
 */

#ifndef ROCMDRRESTRICTEDBATCH_H_
#define ROCMDRRESTRICTEDBATCH_H_

#include <set>
#include <vector>

#include <RocMdrResult.h>
#include <RecursionState.h>
#include <iter/InteractionIterator.h>

class RestrictedIterator
: public InteractionIterator
{
public:
	/**
	 * Constructor.
	 *
	 * @param snps The snps.
	 * @param phenotypes The phenotypes.
	 * @param restricted List of snps that we are restricted to use as
	 *                   the first snp in a multi-snp interaction.
	 * @param interactionOrder The number of snps considered jointly.
	 */
	RestrictedIterator(ColumnData<unsigned char> &snps, PhenotypeMapping &phenotypes,
						  const std::vector<unsigned int> &restricted, unsigned int interactionOrder);

	/**
	 * Constructor for subset iterator.
	 *
	 * @param other Iterator to base the subset iterator on.
	 * @param index Index of this partial iterator.
	 * @param numParts Number of partial iterators.
	 */
	RestrictedIterator(const RestrictedIterator &other, size_t index, size_t numParts);

	/**
	 * Copy constructor.
	 */
	RestrictedIterator(const RestrictedIterator &other);

	/**
	 * Assignment operator.
	 */
	RestrictedIterator & operator=(const RestrictedIterator &other);

	/**
	 * Returns the index that we should start the iteration on
	 * for the snps in the non-restricted set.
	 *
	 * In contrast to AllIterator where we always start on the
	 * last index + 1, here we should start at 0 if we are at
	 * the first recursion level.
	 *
	 * @return the index that we should start the iteration on
	 *         for the snps in the non-restricted set.
	 */
	unsigned int getStartIndex(RecursionState &state);

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
	 * Snps.
	 */
	ColumnData<unsigned char> &m_snps;

	/**
	 * Phenotypes.
	 */
	PhenotypeMapping &m_phenotypes;

	/**
	 * Number of snps considered jointly.
	 */
	unsigned int m_interactionOrder;

	/**
	 * Index of this partial iterator.
	 */
	size_t m_index;

	/**
	 * Total number of partial iterators.
	 */
	size_t m_numParts;

	/**
	 * All RocMdr analysis will always include at least one
	 * snp from this set.
	 */
	std::vector<unsigned int> m_restricted;

	/**
	 * Restricted indices as a set.
	 */
	std::set<unsigned int> m_restrictedAsSet;

};

#endif /* ROCMDRRESTRICTEDBATCH_H_ */
