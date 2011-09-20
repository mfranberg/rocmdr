/*
 * CSVFieldManager.h
 *
 *  Created on: Sep 19, 2011
 *      Author: fmattias
 */

#ifndef CSVFIELDMANAGER_H_
#define CSVFIELDMANAGER_H_

#include <string>

/**
 * This an interface used by the CSVReader that
 * manages the data coming from a CSV file. Specifically
 * it gets a callback when a field is found, or when a
 * row ends.
 */
class CSVProcessor
{
public:
	/**
	 * Called when a field has been parsed.
	 *
	 * @param field The field that has been parsed.
	 */
	virtual void onField(const std::string &field) = 0;

	/**
	 * Called when a rows ends.
	 */
	virtual void onRowEnd() = 0;
};

#endif /* CSVFIELDMANAGER_H_ */
