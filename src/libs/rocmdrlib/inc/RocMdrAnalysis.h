/*
 * RocMdrAnalysis.h
 *
 *  Created on: Jul 7, 2011
 *      Author: fmattias
 */

#ifndef ROCMDRANALYSIS_H_
#define ROCMDRANALYSIS_H_

#include <ColumnData.h>
#include <ColumnHasher.h>
#include <PhenotypeMapping.h>

#include <map>
#include <vector>

/**
 * Calculates statistics related to the area under the ROC curve
 * for the given set of data points.
 */
class RocMdrAnalysis
{
public:
	/**
	 * Small utility class for counting the number of
	 * true and false positives in a cell.
	 */
	struct CellInfo
	{
		/**
		 * Constructor.
		 */
		CellInfo()
		: numTp( 0 ),
		  numFp( 0 ),
		  lastIndividual( 0 )
		{

		}

		/**
		 * Number of true positives.
		 */
		unsigned int numTp;

		/**
		 * Number of false positives.
		 */
		unsigned int numFp;

		/**
		 * The index of the last individual who touched this.
		 */
		unsigned int lastIndividual;
	};

	/**
	 * Constructor.
	 *
	 * @param phenotypes List of phenotypes.
	 * @param data The samples to analyze.
	 */
	RocMdrAnalysis(ColumnData<unsigned int> &data, const PhenotypeMapping &phenotypes);

	/**
	 * Calculates the area under the ROC curve associated with
	 * the given risk factors. Uses the trapezoidal method.
	 *
	 * @return The area under the ROC curve.
	 */
	float calculateAuc();

	/**
	 * Returns a list of the combinations that most likely are associated,
	 * with a 'true' phenotype. I.e. it is sorted according to how much
	 * it is associated with a disease.
	 *
	 * @return A sorted list of combinations.
	 */
	void getCombinations(std::vector< std::vector<unsigned int> > *combinationList);

private:
	/**
	 * Performs the ROC analysis if it is not performed already, if
	 * it has been performed, this function does nothing.
	 *
	 * Note: Updates the state of this class.
	 */
	void performAnalysis();

	/**
	 * Hashes the rows in the given data, so that each row corresponds to
	 * a single comparable element.
	 *
	 * @param data The samples to hash.
	 * @param hashList List of hashed rows.
	 */
	void hashSamples(ColumnData<unsigned int> &data, std::vector<ColumnHasher::hash_type> *hashList);

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
								  std::map<ColumnHasher::hash_type, CellInfo> *cellCounter);

	/**
	 * Produces a list of cells is sorted according to:
	 * true positive rate / false positive rate
	 *
	 * In the case of healthy and sick individuals,
	 * the most sick cells comes first first.
	 *
	 * @param cellCounter A map from row hash to a count
	 *                    of how many samples are in a
	 *                    given cell.
	 * @param cellList The output sorted list of cells, the first
	 *                 one containing the highest fraction
	 *                 of positive to negative cells.
	 */
	void sortCells(const std::map<ColumnHasher::hash_type, CellInfo> &cellCounter, std::vector<CellInfo> *cellList);

	/**
	 * The set of samples.
	 */
	ColumnData<unsigned int> &m_data;

	/**
	 * The phenotype of each individual.
	 */
	const PhenotypeMapping &m_phenotypes;

	/**
	 * The rows of the data set each collapsed
	 * into a unique hash.
	 */
	std::vector<ColumnHasher::hash_type> m_hashList;

	/**
	 * Counts the number of points in a cell depending
	 * on which phenotype they have.
	 */
	std::map<ColumnHasher::hash_type, CellInfo> m_cellCounter;
};

#endif /* ROCMDRANALYSIS_H_ */
