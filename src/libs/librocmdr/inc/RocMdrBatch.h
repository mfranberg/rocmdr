/*
 * RocMdrBatch.h
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */

#ifndef ROCMDRBATCH_H_
#define ROCMDRBATCH_H_

#include <vector>

#include <Filter.h>
#include <RecursionState.h>
#include <RocMdrResult.h>
#include <stat/NullInteraction.h>

/**
 * Represents a set of computations that will be
 * performed on different subsets of a set of SNPs.
 * This batch will do all computations and return
 * a list of results, where each result is bound
 * to a subset of the SNPs.
 */
class RocMdrBatch
{
public:
	/**
	 * Constructor.
	 *
	 * @param snps The SNPs that the batch is based on.
	 * @param phenotypes The phenotypes that the batch is based on.
	 */
	RocMdrBatch(ColumnData<unsigned char> snps, PhenotypeMapping phenotypes);

	/**
	 * Destructor.
	 */
	virtual ~RocMdrBatch();

	/**
	 * Returns the snps associated with this batch.
	 *
	 * @return the snps associated with this batch.
	 */
	ColumnData<unsigned char> getSnps();

	/**
	 * Returns the phenotypes associated with this batch.
	 *
	 * @return the phenotypes associated with this batch.
	 */
	PhenotypeMapping getPhenotypes();

	/**
	 * Assigns a filter that will be considered in the
	 * last level of the recursion.
	 *
	 * RocMdrBatch is responsible for deallocating the filter.
	 *
	 * @param filter The filter to assign.
	 */
	void setFilter(Filter *filter);

	/**
	 * Recursively computes the AUC for all possible
	 * tables of the given interaction order.
	 *
	 * @param interactionOrder Number of snps in each interaction.
	 *                         1 = single snp analysis,
	 *                         2 = all pairs
	 *
	 * @return List of results that contains the AUC for each table
	 *         of the given order.
	 */
	virtual std::vector<RocMdrResult> run(unsigned int interactionOrder);

protected:
	/**
	 * Computes the last step of the recursion in multiple threads.
	 *
	 * @param start The index of the first SNP.
	 * @param end The index of the last SNP.
	 * @param state Recursion state.
	 * @param results List of results.
	 */
	void
	runParallell(unsigned int start,
				 unsigned int end,
				 RecursionState &state,
			   	 std::vector<RocMdrResult> *results);

	/**
	 * Computes the last step of the recursion in a single thread.
	 *
	 * @param start The index of the first SNP.
	 * @param end The index of the last SNP.
	 * @param state Recursion state.
	 * @param results List of results.
	 */
	void
	runSingle(unsigned int start,
			  unsigned int end,
			  RecursionState state,
			  std::vector<RocMdrResult> *results);

private:
	/**
	 * Recursively computes the AUC for all possible
	 * tables of the given order. At each recursive
	 * level a new locus is added, and when we are at
	 * the bottom of the recursion the AUC is computed.
	 *
	 * @param state Current state generated by the recursion.
	 * @param results The list of results that will be produced.
	 */
	void
	runRocMdrRecursive(RecursionState &state,
					   std::vector<RocMdrResult> *results);

	/**
	 * SNPs.
	 */
	ColumnData<unsigned char> m_snps;

	/**
	 * Phenotypes.
	 */
	PhenotypeMapping m_phenotypes;

	/**
	 * A filter that will be used at the last level
	 * of the recursion.
	 */
	Filter *m_filter;

	/**
	 * The number of threads to use.
	 */
	unsigned int m_numThreads;
};

#endif /* ROCMDRBATCH_H_ */
