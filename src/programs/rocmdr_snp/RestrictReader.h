/*
 * RestrictReader.h
 *
 *  Created on: Mar 5, 2012
 *      Author: fmattias
 */

#ifndef RESTRICTREADER_H_
#define RESTRICTREADER_H_

#include <set>
#include <string>

class RestrictReader
{
public:
	/**
	 * Constructor.
	 *
	 * @param path Path to the restrict set.
	 */
	RestrictReader(const char *path);

	/**
	 * Reads the set from the file and returns it.
	 *
	 * @return set of loci.
	 */
	std::set<std::string> readSet();

private:
	/**
	 * Path to the restrict set.
	 */
	std::string m_path;
};

#endif /* RESTRICTREADER_H_ */
