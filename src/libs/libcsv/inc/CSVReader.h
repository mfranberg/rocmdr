/*
 * CSVReader.h
 *
 *  Created on: Sep 19, 2011
 *      Author: fmattias
 */

#ifndef CSVREADER_H_
#define CSVREADER_H_

#include <string>

#include "csv.h"

class CSVProcessor;

static const size_t CSV_BUFFER_SIZE = 4096;

/**
 * This class reads comma separated files and uses
 * a callback interface to supply the data to the user.
 */
class CSVReader
{
public:
	/**
	 * Constructor.
	 *
	 * @param delimiter A string that separates each column.
	 */
	CSVReader(char delimiter = '\t');

	/**
	 * Reads a CSV file and stores the data in the given ColumnData object.
	 *
	 * @param path   Path to the file to read.
	 * @param processor The parsed input is processed by this class.
	 *
	 * @return True if the file was read successfully, false otherwise.
	 */
	bool parse(const std::string path, CSVProcessor *processor);

	/**
	 * Reads a CSV file from a stream and stores the data in the given
	 * ColumnData object.
	 *
	 * @param input File stream to read from.
	 * @param processor The parsed input is processed by this class.
	 *
	 * @return True if the file was read successfully, false otherwise.
	 */
	bool parse(FILE *input, CSVProcessor *processor);

	/**
	 * Returns the processor used in the last call to parse.
	 *
	 * @return the processor used in the last call to parse.
	 */
	CSVProcessor *getCurrentProcessor();

private:
	/**
	 * Character delimiter.
	 */
	char m_delimiter;

	/**
	 * Currently assigned
	 */
	CSVProcessor *m_currentProcessor;
};

#endif /* CSVREADER_H_ */
