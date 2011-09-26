/*
 * pvalues.h
 *
 *  Created on: Sep 26, 2011
 *      Author: fmattias
 */

#ifndef PVALUES_H_
#define PVALUES_H_

#include <vector>

/**
 * Simulates p-values from a uniform null-hypothesis and computes
 * a empirical p-value for each of the given AUC values. It also saves
 * the simulations in a file, since the computations can be quite
 * intense.
 *
 * @param aucValues The AUC values to compute p-values for.
 * @param columnRanges The number of distinct values in each column.
 * @param numSamples The length of each column.
 * @param filename The filename where the simulations will be stored.
 *
 * @return A list of p-values for each AUC-value.
 */
std::vector<float>
simulatePValues(const std::vector<float> &aucValues,
				const std::vector<unsigned int> &columnRanges,
				unsigned int numSamples,
				const char *filename);


#endif /* PVALUES_H_ */
