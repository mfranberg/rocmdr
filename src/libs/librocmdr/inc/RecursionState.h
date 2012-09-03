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
	 * Copy constructor.
	 *
	 * @param other Other state to initialize from.
	 */
	RecursionState(const RecursionState &other);

	/**
	 * Assignment operator.
	 *
	 * @param other Other object to assign to this.
	 */
	RecursionState & operator=(const RecursionState &other);

	/**
	 * Pushes a SNP to the recursion state.
	 *
	 * @param index Index to a column in the SNPs.
	 */
	void push(unsigned int index);

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
	 * Returns the last index if there is one, otherwise
	 * -1 is returned.
	 *
	 * @return The last index if there is one, otherwise
	 * -1 is returned.
	 */
	unsigned int getLastIndex();

	/**
	 * Returns the current depth of the recursion state.
	 *
	 * @return the current depth of the recursion state.
	 */
	unsigned int depth();

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
	ColumnData<unsigned char> &m_snps;

	/**
	 * The SNPs currently in the state.
	 */
	ColumnData<unsigned char> m_curSnps;

	/**
	 * Current set of indices.
	 */
	std::vector<unsigned int> m_curIndex;
};

#endif /* RECURSIONSTATE_H_ */
