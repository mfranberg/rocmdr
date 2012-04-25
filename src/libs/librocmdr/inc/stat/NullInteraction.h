/*
 * NullInteraction.h
 *
 *  Created on: Apr 5, 2012
 *      Author: fmattias
 */

#ifndef NULLINTERACTION_H_
#define NULLINTERACTION_H_

#include <data/ColumnData.h>
#include <data/PhenotypeMapping.h>

/**
 * Simulates the null hypothesis that all SNPs are
 * independently associated with the phenotype. It does
 * this by conditioning on the phenotype and shuffling
 * each SNP independently. This will destroy any relationship
 * between the SNPs but keeping the marginal densities the
 * same.
 */
class NullInteraction
{
public:
	/**
	 * Constructor.
	 *
	 * @param numIters The number of iterations.
	 */
	NullInteraction(unsigned int numIters);

	/**
	 * Returns the p-value for the g
	 *
	 * @param AUC achieved under the alternative hypothesis.
	 * @param data Data for which the above AUC was achieved.
	 * @param phenotypes Phenotypes.
	 *
	 * @return The p-value for the given auc under the null hypothesis.
	 */
	float getPValue(float auc, ColumnData<unsigned char> &data, const PhenotypeMapping &phenotypes);

private:
	/**
	 * Number of iterations to perform.
	 */
	unsigned int m_numIters;
};

#endif /* NULLINTERACTION_H_ */
