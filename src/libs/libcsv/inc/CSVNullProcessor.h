/*
 * CSVNullProcessor.h
 *
 *  Created on: Sep 20, 2011
 *      Author: fmattias
 */

#ifndef CSVNULLPROCESSOR_H_
#define CSVNULLPROCESSOR_H_

#include "CSVProcessor.h"

/**
 * Default null processor, does nothing.
 */
class CSVNullProcessor
{
public:
	/**
	 * @see CSVProcessor::onField.
	 */
	void onField(const std::string &field)
	{

	}

	/**
	 * @see CSVProcessor::onRowEnd.
	 */
	void onRowEnd()
	{

	}
};

#endif /* CSVNULLPROCESSOR_H_ */

