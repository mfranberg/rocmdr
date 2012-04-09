/*
 * RestrictFilter.h
 *
 *  Created on: Apr 9, 2012
 *      Author: fmattias
 */

#ifndef RESTRICTFILTER_H_
#define RESTRICTFILTER_H_

#include <set>
#include <vector>

#include <Filter.h>

/**
 * Skips all SNPs in a given set.
 */
class RestrictFilter
: public Filter
{
public:
	/**
	 * Constructor.
	 *
	 * @param skipList List of snps to skip.
	 */
	RestrictFilter(const std::vector<unsigned int> &skipList);

	/**
	 * @see Filter::skip.
	 */
	bool skip(unsigned int index);

private:
	/**
	 * Contains a set of indices to skip.
	 */
	std::set<unsigned int> m_skipSet;
};

#endif /* RESTRICTFILTER_H_ */
