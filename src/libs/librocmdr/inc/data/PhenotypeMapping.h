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
	 */
	PhenotypeMapping();

	/**
	 * Constructor.
	 *
	 * @param phenotypes The phenotypes.
	 */
	PhenotypeMapping(const std::vector<bool> &phenotypes);

	/**
	 * Copy constructor.
	 *
	 * @param other Other mapping to initialize from.
	 */
	PhenotypeMapping(const PhenotypeMapping &other);

	/**
	 * Assignment operator.
	 *
	 * @param other Other object to assign to this.
	 */
	PhenotypeMapping & operator=(const PhenotypeMapping &other);

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
	 * Returns a list of positive indices.
	 *
	 * @return a list of positive indices.
	 */
	std::vector<unsigned int> getPositiveIndices() const;

	/**
	 * Returns a list of negative indices.
	 *
	 * @return a list of negative indices.
	 */
	std::vector<unsigned int> getNegativeIndices() const;

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
	std::vector<bool> m_phenotypes;

	/**
	 * List of positive indices.
	 */
	std::vector<unsigned int> m_positive;

	/**
	 * List of negative indices.
	 */
	std::vector<unsigned int> m_negative;
};

#endif /* PHENOTYPEMAPPING_H_ */
