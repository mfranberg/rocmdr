/*
 * RocMdrBatch.h
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */

#ifndef ROCMDRBATCH_H_
#define ROCMDRBATCH_H_

#include <vector>

#include <RocMdrResult.h>
#include <NoAssociationSimulator.h>

class RocMdrBatch
{
public:
	/**
	 * Recursively computes the AUC for all possible
	 * tables of the given order. At each recursive
	 * level a new locus is added, and when we are at
	 * the bottom of the recursion the AUC is computed.
	 *
	 * @param maxLevel Order of the interaction (dimension of the tables).
	 * @param allSnps All SNPs such that each column contains the SNPs for
	 *                a single loci.
	 * @param phenotypes Disease status of the individuals.
	 * @param curSnps The partial mdr table at this level of the recursion.
	 * @param curLoci The loci associated with the table.
	 * @param results The list of results that will be produced.
	 */
	void
	runRocMdrRecursive(unsigned int maxLevel,
					   ColumnData<unsigned char> &allSnps,
					   PhenotypeMapping &phenotypes,
					   ColumnData<unsigned char> &curSnps,
					   std::vector<unsigned int> &curLoci,
					   std::vector<RocMdrResult> *results);

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
	std::vector<RocMdrResult> run(unsigned int interactionOrder,
									   ColumnData<unsigned char> snps,
									   PhenotypeMapping phenotypes);

private:
	/**
	 * Simulates the AUC under the null hypothesis.
	 */
	NoAssociationSimulator m_nullSimulator;
};


#endif /* ROCMDRBATCH_H_ */
