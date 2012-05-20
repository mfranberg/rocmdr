/*
 * RocMdrPairBatch.h
 *
 *  Created on: Apr 25, 2012
 *      Author: fmattias
 */

#ifndef ROCMDRPAIRBATCH_H_
#define ROCMDRPAIRBATCH_H_

#include <set>
#include <vector>

#include <Filter.h>
#include <RocMdrBatch.h>
#include <RocMdrResult.h>
#include <RecursionState.h>
#include <stat/NullInteraction.h>

class RocMdrPairBatch
: public RocMdrBatch
{
public:
	/**
	 * Constructor.
	 *
	 * @param order Number of SNPs considered jointly from the
	 *              restricted set
	 */
	RocMdrPairBatch(ColumnData<unsigned char> snps,
					PhenotypeMapping phenotypes,
					const std::vector<std::pair<unsigned int, unsigned int> > &pairs);

	/**
	 * Runs rocmdr for each pair.
	 *
	 * @return List of results that contains the AUC for each table
	 *         of the given order.
	 */
	std::vector<RocMdrResult> run(unsigned int interactionOrder);

	void runSingle(unsigned int start, unsigned int end, std::vector<RocMdrResult> *results);
	void runParallell(unsigned int start, unsigned int end, std::vector<RocMdrResult> *results);

private:

	/**
	 * The pairs to analyze.
	 */
	std::vector<std::pair<unsigned int, unsigned int> > m_pairs;

};

#endif /* ROCMDRPAIRBATCH_H_ */
