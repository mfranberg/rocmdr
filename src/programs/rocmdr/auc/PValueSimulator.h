/*
 * PValueSimulator.h
 *
 *  Created on: Sep 22, 2011
 *      Author: fmattias
 */

#ifndef PVALUESIMULATOR_H_
#define PVALUESIMULATOR_H_

#include <vector>
#include <ColumnData.h>

/**
 * Number of iterations in the simulation.
 */
static const unsigned int SIMULATION_ITERATIONS = 2000;

/**
 * Simulates a set of p-values.
 */
class PValueSimulator
{
public:
	/**
	 * Constructor.
	 *
	 * @param columnRanges Vector containing the number of bins in each column.
	 * @param numSamples Number of samples.
	 */
	PValueSimulator(const std::vector<unsigned int> &columnRanges, unsigned int numSamples);

	/**
	 * Computes the P-value for the given AUC by simulation.
	 *
	 * @param auc The AUC to compute the p-value for.
	 *
	 * @return The p-value for the given AUC.
	 */
	float computePValue(float auc);

	/**
	 * Sets the simulated AUC values to the given vector.
	 *
	 * Note: Any further p-value computation will be based
	 *       on these values.
	 *
	 * @param simulation The AUC values to use.
	 */
	void setSimulation(std::vector<float> simulation);

	/**
	 * Returns the current vector of simulated values.
	 *
	 * @return the current vector of simulated values.
	 */
	const std::vector<float> &getSimulation();

private:
	/**
	 * Generates random data for a set of columns in the given range.
	 *
	 * @param columnRanges Vector containing the number of bins in each column.
	 * @param numSamples Length of each column.
	 * @param data The random data will be stored here.
	 */
	void generateRandomData(std::vector<unsigned int> columnRanges, unsigned int numSamples, ColumnData<unsigned int> *data);

	/**
	 * Generates uniform phenotypes.
	 *
	 * @param numSamples The number of phenotypes to generate.
	 * @param phenotypes The random phenotypes will be stored here.
	 */
	void generatePhenotypes(unsigned int numSamples, std::vector<bool> *phenotypes);

	/**
	 * Range of each column.
	 */
	std::vector<unsigned int> m_columnRanges;

	/**
	 * The number of samples.
	 */
	unsigned int m_numSamples;

	/**
	 * Vector of simulated AUC values under the null
	 * hypothesis.
	 */
	std::vector<float> m_simulatedAuc;

};


#endif /* PVALUESIMULATOR_H_ */
