/*
 * RocMdrResult.h
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */

#ifndef ROCMDRRESULT_H_
#define ROCMDRRESULT_H_

#include <vector>

/**
 * Utility class that holds the result of an RocMdr run.
 */
class RocMdrResult
{
public:
	/**
	 * Constructor.
	 */
	RocMdrResult(std::vector<unsigned int> loci, float auc, float pValue);

	/**
	 * Copy constructor.
	 */
	RocMdrResult(const RocMdrResult &other);

	/**
	 * Assignment operator.
	 */
	RocMdrResult & operator=(const RocMdrResult &other);

	/**
	 * Adds a locus to the list of loci.
	 *
	 * @param locus The locus to add.
	 */
	void addLocus(unsigned int locus);

	/**
	 * Removes the last added locus.
	 */
	void removeLastLocus();

	/**
	 * Returns the loci for this result.
	 *
	 * @return the loci for this result.
	 */
	std::vector<unsigned int> getLoci() const;

	/**
	 * Sets the p-value of the given set of
	 * loci.
	 *
	 * @param auc The AUC of the set of loci.
	 * @param pValue The p-value of the set of loci.
	 */
	void setResult(float auc, float pValue);

	/**
	 * Returns the p-value of the set of loci.
	 *
	 * @return the p-value of the set of loci.
	 */
	double getPValue() const;

	/**
	 * Returns the AUC of the set of loci.
	 */
	float getAuc() const;

private:
	/**
	 * The loci for which the interaction was
	 * computed.
	 */
	std::vector<unsigned int> m_loci;

	/**
	 * The computed AUC.
	 */
	float m_auc;

	/**
	 * The p-value of the AUC.
	 */
	double m_pValue;
};

#endif /* ROCMDRRESULT_H_ */
