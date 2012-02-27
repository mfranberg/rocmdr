/*
 * PlinkIo.h
 *
 *  Created on: Feb 8, 2012
 *      Author: fmattias
 */

#ifndef PLINKIO_H_
#define PLINKIO_H_

#include <string>
#include <vector>

extern "C"
{
	#include <plinkio/plinkio.h>
}

#include <data/ColumnData.h>

class PlinkIo
{
public:
	/**
	 * Constructor.
	 *
	 * Opens the given plink path or throws an
	 * exception if it could not be opened.
	 *
	 * @param plinkPath Path to the plink file.
	 */
	PlinkIo(const std::string &plinkPath);

	/**
	 * Returns a ColumnData object containing all SNPs. In
	 * which each column is the genotypes for all individuals
	 * at a single locus.
	 *
	 * @return a ColumnData object containing all SNPs.
	 */
	ColumnData<unsigned char> getSnps();

	/**
	 * Returns a list of phenotypes for all individuals. The i:th
	 * phenotype is for the i:th individual in a column.
	 *
	 * @return a list of phenotypes for all individuals.
	 */
	std::vector<bool> getPhenotypes();

	/**
	 * Returns the i:th locus, in the same order as
	 * returned by getSnps.
	 */
	std::string getLocus(unsigned int i);

	/**
	 * Closes the file if it is open, otherwise
	 * does nothing.
	 */
	void close();

	/**
	 * Destructor.
	 *
	 * Closes the file if it still is open.
	 */
	virtual ~PlinkIo();

private:
	/**
	 * Determines whether the file is open or not.
	 */
	bool m_isOpen;

	/**
	 * Underlying C plink file.
	 */
	struct pio_file_t m_plinkFile;
};

#endif /* PLINKIO_H_ */
