/*
 * Filter.h
 *
 *  Created on: Apr 9, 2012
 *      Author: fmattias
 */

#ifndef FILTER_H_
#define FILTER_H_

/**
 * Filter used in RocMdrBatch to filter out some
 * SNPs, for example when a specific subset should
 * not be used.
 */
class Filter
{
public:
	/**
	 * Returns true if this index should be skipped.
	 *
	 * @param index The index of the SNP.
	 *
	 * @return true if the index should be skipped.
	 */
	virtual bool skip(unsigned int index) = 0;
};

#endif /* FILTER_H_ */
