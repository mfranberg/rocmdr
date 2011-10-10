/*
 * NoAssociationSimulator.h
 *
 *  Created on: Oct 7, 2011
 *      Author: fmattias
 */

#ifndef NOASSOCIATIONSIMULATOR_H_
#define NOASSOCIATIONSIMULATOR_H_

#include <vector>

#include <ColumnData.h>
#include <PhenotypeMapping.h>

/**
 * Default number of iterations in the simulation.
 */
static const unsigned int SIMULATION_ITERATIONS = 2000;

/**
 * Simulates AUCs from a set of factors, assuming that SNPs
 * and factors are independent given the phenotype.
 */
class NoAssociationSimulator
{
public:
	/**
	 * Constructor.
	 *
	 * @param columnRanges Vector containing the number of bins in each column.
	 * @param numSamples Number of samples.
	 */
	NoAssociationSimulator(ColumnData<unsigned int> &factors, const PhenotypeMapping &phenotypes);

	/**
	 * Computes the P-value for the given AUC by simulation.
	 *
	 * @param auc The AUC to compute the p-value for.
	 *
	 * @return The p-value for the given AUC.
	 */
	float computePValue(float auc);

	/**
	 * Sets the number of samples to use in the simulation.
	 *
	 * @param numSamples the number of samples to use.
	 */
	void setNumberOfSamples(unsigned int numSamples);

private:
	void runSimulation();

	/**
	 * Generates random data for a set of columns in the given range.
	 *
	 * @param numSnps The number of snps to simulate.
	 * @param simulatedSnp The generated snps will be stored here.
	 */
	void generateSnp(unsigned int numSnps, std::vector<unsigned int> *simulatedSnp);

	/**
	 * The set of factors given as input data.
	 */
	ColumnData<unsigned int> m_factors;

	/**
	 * The phenotypes associated with the factors.
	 */
	const PhenotypeMapping &m_phenotypes;

	/**
	 * The number of samples.
	 */
	unsigned int m_numSamples;

	/**
	 * Vector of simulated AUC values under the no association
	 * hypothesis.
	 */
	std::vector<float> m_simulatedAuc;
};


#endif /* NOASSOCIATIONSIMULATOR_H_ */
