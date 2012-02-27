/*
 * RocMdrAnalysis.h
 *
 *  Created on: Jul 7, 2011
 *      Author: fmattias
 */

#ifndef ROCMDRANALYSIS_H_
#define ROCMDRANALYSIS_H_

#include <Cell.h>
#include <data/ColumnData.h>
#include <data/ColumnHasher.h>
#include <data/PhenotypeMapping.h>

#include <map>
#include <vector>
#include <google/dense_hash_map>

/**
 * Calculates statistics related to the area under the ROC curve
 * for the given set of data points.
 */
class RocMdrAnalysis
{
public:
	typedef unsigned char column_type;

	/**
	 * Constructor.
	 *
	 * @param phenotypes List of phenotypes.
	 * @param data The samples to analyze.
	 */
	RocMdrAnalysis(ColumnData<column_type> &data, const PhenotypeMapping &phenotypes);

	/**
	 * Calculates the area under the ROC curve associated with
	 * the given risk factors. Uses the trapezoidal method.
	 *
	 * @return The area under the ROC curve.
	 */
	float getAuc();

	/**
	 * Returns the p-value for the AUC.
	 *
	 * The p-value is computed through the Mann-Whitney-Hugh test
	 * with ties.
	 *
	 * @return the p-value for the AUC.
	 */
	double getPValue();

	/**
	 * Returns a list of the combinations that most likely are associated,
	 * with a 'true' phenotype. I.e. it is sorted according to how much
	 * it is associated with a disease.
	 *
	 * @return A sorted list of combinations.
	 */
	void getCombinations(std::vector< std::vector<column_type> > *combinationList);

private:
	/**
	 * Hashes the rows in the given data, so that each row corresponds to
	 * a single comparable element.
	 *
	 * @param data The samples to hash.
	 * @param hashList List of hashed rows.
	 */
	void hashSamples(ColumnData<column_type> &data, std::vector<ColumnHasher::hash_type> *hashList);

	/**
	 * Counts the number of samples with the different
	 * phenotypes in each cell.
	 *
	 * @param phenotypes The phenotype mapping of the samples.
	 * @param hashList A list of hashes of the rows.
	 * @param cellCounter A map that is used to
	 */
	void countSamplesInEachCell(const PhenotypeMapping &phenotypes,
								  const std::vector<ColumnHasher::hash_type> &hashList,
								  google::dense_hash_map<ColumnHasher::hash_type, Cell> *cellCounter);

	/**
	 * Produces a list of cells is sorted according to:
	 * true positive rate / false positive rate
	 *
	 * In the case of healthy and sick individuals,
	 * the most sick cells comes first first.
	 *
	 * Note: If m_sortedCells is non-empty, the sorting is already done,
	 *       so this function does nothing.
	 */
	void sortCells();

	/**
	 * The set of samples.
	 */
	ColumnData<column_type> m_data;

	/**
	 * The phenotype of each individual.
	 */
	PhenotypeMapping m_phenotypes;

	/**
	 * List of cells sorted according to their slope in the
	 * corresponding ROC curve.
	 */
	std::vector<Cell> m_sortedCells;
};

#endif /* ROCMDRANALYSIS_H_ */
