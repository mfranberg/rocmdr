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

class RocMdrBatch
{
public:
	/**
	 * Constructor.
	 */
	RocMdrBatch();

	/**
	 * Sets the number of threads to use.
	 */
	void setNumThreads(unsigned int numThreads);

	/**
	 * Returns the number of threads to use when
	 * executing the batch.
	 *
	 * @return the number of threads to use.
	 */
	unsigned int getNumThreads();

	/**
	 * Recursively computes the AUC for all possible
	 * tables of the given interaction order.
	 *
	 * @param snps All SNPs such that each column contains the SNPs for
	 *             a single loci.
	 * @param phenotypes Disease status of the individuals.
	 *
	 * @return List of results that contains the AUC for each table
	 *         of the given order.
	 */
	virtual std::vector<RocMdrResult> run(unsigned int interactionOrder,
								          ColumnData<unsigned char> snps,
								          PhenotypeMapping phenotypes);

private:
	/**
	 * Recursively computes the AUC for all possible
	 * tables of the given order. At each recursive
	 * level a new locus is added, and when we are at
	 * the bottom of the recursion the AUC is computed.
	 *
	 * @param state Current state generated by the recursion.
	 * @param snps  All SNPs such that each column contains the SNPs for
	 *                a single loci.
	 * @param phenotypes Disease status of the individuals.
	 * @param results The list of results that will be produced.
	 */
	void
	runRocMdrRecursive(RecursionState &state,
					   ColumnData<unsigned char> &snps,
					   PhenotypeMapping &phenotypes,
					   std::vector<RocMdrResult> *results);

	/**
	 * The number of threads to use.
	 */
	unsigned int m_numThreads;
};

/**
 * Computes the last step of the recursion in multiple threads.
 *
 * @param start The index of the first SNP.
 * @param end The index of the last SNP.
 * @param state Recursion state.
 * @param filter SNPs to filter out.
 * @param phenotypes The phenotypes.
 * @param results List of results.
 * @param numThreads The number of threads.
 */
void
runParallell(unsigned int start,
			 unsigned int end,
			 RecursionState &state,
			 Filter *filter,
		   	 PhenotypeMapping &phenotypes,
		   	 std::vector<RocMdrResult> *results,
		   	 unsigned int numThreads);

/**
 * Computes the last step of the recursion in a single thread.
 *
 * @param start The index of the first SNP.
 * @param end The index of the last SNP.
 * @param state Recursion state.
 * @param filter SNPs to filter out.
 * @param phenotypes The phenotypes.
 * @param results List of results.
 */
void
runSingle(unsigned int start,
		  unsigned int end,
		  RecursionState state,
		  Filter *filter,
		  PhenotypeMapping &phenotypes,
		  std::vector<RocMdrResult> *results);


#endif /* ROCMDRBATCH_H_ */
