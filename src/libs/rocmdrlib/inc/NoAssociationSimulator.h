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
 * Simulates AUCs from a set of factors, assuming that SNPs
 * and factors are independent given the phenotype.
 */
class NoAssociationSimulator
{
public:
	/**
	 * Constructor.
	 */
	NoAssociationSimulator();

	/**
	 * Constructor.
	 *
	 * Simulates the null-hypothesis to approximate p-values. The null-hypothesis
	 * says that the SNPs are independent of the phenotype.
	 *
	 * @param snps The snps that will be tested for association.
	 * @param phenotypes The phenotypes that might be associated with the snps.
	 */
	NoAssociationSimulator(ColumnData<unsigned char> &snps, const std::vector<bool> &phenotypes);

	/**
	 * Sets the data to simulate from, the following p-values will
	 * be based on this data.
	 *
	 * @param snps The snps that will be tested for association.
	 * @param phenotypes The phenotypes that might be associated with the snps.
	 */
	void setData(ColumnData<unsigned char> &snps, const std::vector<bool> &phenotypes);

	/**
	 * Set the order to simulate.
	 *
	 * @param order The order of the interactions.
	 */
	void setOrder(unsigned int order);

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
	/**
	 * Simulates the null-hypothesis that there is no association
	 * with any given snp and the disease.
	 */
	void runSimulation();

	/**
	 * Generates random data for a set of columns in the given range.
	 *
	 * @param numSnps The number of snps to simulate.
	 * @param simulatedSnp The generated snps will be stored here.
	 */
	void generateSnp(unsigned int numSnps, std::vector<unsigned int> *simulatedSnp);

	/**
	 * Shuffles the given phenotypes using the Fisher-Yates algorithm.
	 *
	 * @param phenotypes Phenotypes to shuffle.
	 */
	void shufflePhenotypes(std::vector<bool> *phenotypes);

	/**
	 * Returns a uniformly distributed random integer in [0, upper], i.e. inclusive.
	 *
	 * @return a uniformly distributed random integer in the given interval.
	 */
	unsigned int getRandomInteger(unsigned int upper);

	/**
	 * Computes the number of unique combinations of snps with
	 * the given interactions order.
	 *
	 * @param interactionOrder The order of the interactions.
	 * @param numSnps The snps that will be tested for association.
	 *
	 * @return The number of unique combinations.
	 */
	unsigned int numCombinations(unsigned int interactionOrder, unsigned int numSnps);

	/**
	 * The snps that will be tested for association. Stored here
	 * to avoid all work being done in the constructor.
	 */
	ColumnData<unsigned char> m_snps;

	/**
	 * The phenotypes that might be associated with the snps. Stored here
	 * to avoid all work being done in the constructor.
	 */
	std::vector<bool> m_phenotypes;

	/**
	 * The number of null hypotheses to simulate.
	 */
	unsigned int m_numSamples;

	/**
	 * Vector of simulated AUC values under the no association
	 * hypothesis.
	 */
	std::vector<float> m_simulatedAuc;

	/**
	 * The interaction order.
	 */
	unsigned int m_order;
};


#endif /* NOASSOCIATIONSIMULATOR_H_ */
