/*
 * PairReader.h
 *
 *  Created on: Apr 25, 2012
 *      Author: fmattias
 */

#ifndef PAIRREADER_H_
#define PAIRREADER_H_

#include <string>
#include <vector>

/**
 * Reads a set of SNP pairs delimited by
 * newlines.
 */
class PairReader
{
public:
	/**
	 * Constructor.
	 *
	 * @param path Path to the pairs to read.
	 */
	PairReader(const char *path);

	/**
	 * Reads the set of pairs from the file and returns it.
	 *
	 * @return set of loci.
	 */
	std::vector< std::pair<std::string, std::string> > readSet();

private:
	/**
	 * Path to the restrict set.
	 */
	std::string m_path;
};

#endif /* PAIRREADER_H_ */
