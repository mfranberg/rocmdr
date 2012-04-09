/*
 * NullFilter.h
 *
 *  Created on: Apr 9, 2012
 *      Author: fmattias
 */

#ifndef NULLFILTER_H_
#define NULLFILTER_H_

#include <Filter.h>

/**
 * No SNPs are skipped in this filter. Used by default
 * in RocMdrBatch.
 */
class NullFilter
: public Filter
{
public:
	/**
	 * @see Filter::skip.
	 */
	bool skip(unsigned int index);

};

#endif /* NULLFILTER_H_ */
