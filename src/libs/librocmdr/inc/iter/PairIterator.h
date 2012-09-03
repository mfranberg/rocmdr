/*
 * RocMdrPairBatch.h
 *
 *  Created on: Apr 25, 2012
 *      Author: fmattias
 */

#ifndef ROCMDRPAIRBATCH_H_
#define ROCMDRPAIRBATCH_H_

#include <vector>

#include <iter/InteractionIterator.h>
#include <RocMdrResult.h>
#include <RecursionState.h>

class PairIterator
: public InteractionIterator
{
public:
	/**
	 * Constructor.
	 *
	 * @param snps The snps.
	 * @param phenotypes The phenotypes.
	 * @param pairs List of snp pairs to iterate over.
	 */
	PairIterator(ColumnData<unsigned char> &snps,
					PhenotypeMapping &phenotypes,
					const std::vector<std::pair<unsigned int, unsigned int> > &pairs);

	/**
	 * Constructor for subset iterator.
	 *
	 * @param other Iterator to base the subset iterator on.
	 * @param index Index of this partial iterator.
	 * @param numParts Number of partial iterators.
	 */
	PairIterator(const PairIterator &other, size_t index, size_t numParts);

	/**
	 * Copy constructor.
	 */
	PairIterator(const PairIterator &other);

	/**
	 * Assignment operator.
	 */
	PairIterator & operator=(const PairIterator &other);

	/**
	 * @see InteractionIterator::visitAll.
	 */
	void visitAll(InteractionVisitor *visitor);

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
	 * Index of this partial iterator.
	 */
	size_t m_index;

	/**
	 * Number of partial iterators.
	 */
	size_t m_numParts;

	/**
	 * The pairs to analyze.
	 */
	std::vector<std::pair<unsigned int, unsigned int> > m_pairs;

};

#endif /* ROCMDRPAIRBATCH_H_ */
