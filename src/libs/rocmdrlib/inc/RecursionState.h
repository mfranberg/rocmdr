/*
 * RecursionState.h
 *
 *  Created on: Feb 28, 2012
 *      Author: fmattias
 */

#ifndef RECURSIONSTATE_H_
#define RECURSIONSTATE_H_

#include <data/ColumnData.h>
#include <data/PhenotypeMapping.h>

/**
 * Represents a state in the recursion.
 */
class RecursionState
{
public:
	/**
	 *
	 * @param maxDepth Maximum depth of the recursion (number
	 *                 of SNPs considered jointly).
	 * @param snps The snps.
	 * @param phenotypes The phenotypes.
	 */
	RecursionState(unsigned int maxDepth, ColumnData<unsigned char> &snps);

	/**
	 * Pushes a SNP to the recursion state.
	 *
	 * @param index Index to a column in the SNPs.
	 */
	void push(unsigned int index);

	/**
	 * Pushes a SNP to the recursion state, but in addition
	 * it stores a virtual index, that is used instead of index
	 * when calling lastIndex. This can be useful when you have
	 * a partial list of SNPs that you want to loop over, but want
	 * to access them from the global list of all snps.
	 *
	 * @param index Index to a column in the SNPs.
	 * @param virutalIndex Virtual index that will be returned instead
	 *                     of index by nextIndex.
	 *
	 */
	void push(unsigned int index, unsigned int virtualIndex);

	/**
	 * Pops the last SNP from the recursion state.
	 */
	void pop();

	/**
	 * Returns the SNPs currently in the state.
	 *
	 * Note a reference is returned, so any modifications
	 * will be reflected in the state as well.
	 *
	 * @return the SNPs currently in the state.
	 */
	ColumnData<unsigned char> &getCurrentSnps();

	/**
	 * Returns the indices of the SNPs currently in the state.
	 *
	 * Note a reference is returned, so any modifications
	 * will be reflected in the state as well.
	 *
	 * @return the indices of the SNPs currently in the state.
	 */
	std::vector<unsigned int> &getCurrentIndices();

	/**
	 * Returns the successor of the last pushed index. If a virtual index
	 * exist then its successor will be returned instead. If nothing is
	 * pushed, 0 will be returned.
	 *
	 * @return the successor index.
	 */
	unsigned int nextIndex();

	/**
	 * Returns true if the recursion is at its max depth.
	 *
	 * @return true if the recursion is at its max depth.
	 */
	bool done();

private:
	/**
	 * Max depth of the recursion.
	 */
	unsigned int m_maxDepth;

	/**
	 * Set of SNPs associated with the recursion state.
	 */
	ColumnData<unsigned char> m_snps;

	/**
	 * The SNPs currently in the state.
	 */
	ColumnData<unsigned char> m_curSnps;

	/**
	 * Current set of indices.
	 */
	std::vector<unsigned int> m_curIndex;

	/**
	 * Last index that was added.
	 */
	std::vector<unsigned int> m_lastIndex;
};

#endif /* RECURSIONSTATE_H_ */
