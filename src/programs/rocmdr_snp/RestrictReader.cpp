/*
 * RestrictReader.cpp
 *
 *  Created on: Mar 5, 2012
 *      Author: fmattias
 */
#include <fstream>

#include "RestrictReader.h"

RestrictReader::RestrictReader(const char *path)
: m_path( path )
{

}

std::set<std::string>
RestrictReader::readSet()
{
	std::ifstream restrictFile( m_path );

	std::set<std::string> restrictSet;
	while( restrictFile.good( ) )
	{
		std::string lociName;
		restrictFile >> lociName;
		restrictSet.insert( lociName );
	}

	return restrictSet;
}
