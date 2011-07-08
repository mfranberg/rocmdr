/*
 * PhenotypeMapping.h
 *
 *  Created on: Jul 8, 2011
 *      Author: fmattias
 */

#ifndef PHENOTYPEMAPPING_H_
#define PHENOTYPEMAPPING_H_

#include <vector>

/**
 * Represents a mapping between data and phenotypes.
 */
class PhenotypeMapping
{
public:
	/**
	 * Constructor.
	 *
	 * @param phenotypes The phenotypes.
	 */
	PhenotypeMapping(const std::vector<bool> &phenotypes);

	/**
	 * Returns the number of positive samples.
	 *
	 * @return the number of positive samples.
	 */
	unsigned int getNumPositive() const;

	/**
	 * Returns the number of negative samples.
	 */
	unsigned int getNumNegative() const;

	/**
	 * Returns the phenotypes of the data.
	 *
	 * @return the phenotypes of the data.
	 */
	const std::vector<bool> & getPhenotypes() const;

	/**
	 * Returns true if the sample with the given index
	 * is regarded as positive.
	 *
	 * @param index The index of the sample to check.
	 * @return true if the samples with the given index is
	 *              positive, false otherwise.
	 */
	bool isPositive(unsigned int index) const;

	/**
	 * Returns the number of phenotypes.
	 *
	 * @return The number of individuals.
	 */
	unsigned int size() const;

private:

	/**
	 * The phenotypes.
	 */
	const std::vector<bool> &m_phenotypes;
};

#endif /* PHENOTYPEMAPPING_H_ */
