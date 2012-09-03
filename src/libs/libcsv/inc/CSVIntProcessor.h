/*
 * CSVIntProcessor.h
 *
 *  Created on: Sep 20, 2011
 *      Author: fmattias
 */

#ifndef CSVINTPROCESSOR_H_
#define CSVINTPROCESSOR_H_

#include <vector>

#include "CSVProcessor.h"

/**
 * Parses integer CSV data.
 */
class CSVIntProcessor
: public CSVProcessor
{
public:
	/**
	 * Constructor.
	 */
	CSVIntProcessor();

	/**
	 * Destructor.
	 */
	~CSVIntProcessor();

	/**
	 * @see CSVProcessor::onField.
	 */
	void onField(const std::string &field);

	/**
	 * @see CSVProcessor::onRowEnd.
	 */
	void onRowEnd();

	/**
	 * Returns the read columns.
	 *
	 * @return the read columns.
	 */
	std::vector< std::vector< char > > getColumnData();

	/**
	 * Returns the column header.
	 *
	 * @return the column header.
	 */
	std::vector< std::string > getHeader();

private:
	/**
	 * Current row number.
	 */
	unsigned int m_row;

	/**
	 * Current column.
	 */
	unsigned int m_column;

	/**
	 * The names of the columns.
	 */
	std::vector< std::string > m_columnHeader;

	/**
	 * The values that we have read.
	 */
	std::vector< std::vector< char > > m_columns;

	/**
	 * Values int the current row.
	 */
	std::vector< char > m_currentRow;
};

#endif /* CSVINTPROCESSOR_H_ */
