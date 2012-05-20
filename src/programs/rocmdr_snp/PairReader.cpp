/*
 * PairReader.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: fmattias
 */

#include <fstream>

#include "PairReader.h"

PairReader::PairReader(const char *path)
: m_path( path )
{

}

std::vector< std::pair<std::string, std::string> >
PairReader::readSet()
{
	std::ifstream pairFile( m_path.c_str( ) );

	std::vector< std::pair<std::string, std::string> > pairs;
	while( pairFile.good( ) )
	{
		std::string loci1;
		std::string loci2;
		pairFile >> loci1;
		pairFile >> loci2;

		if( loci1.empty( ) || loci2.empty( ) )
		{
			continue;
		}

		pairs.push_back( std::pair<std::string, std::string>( loci1, loci2 ) );
	}

	return pairs;
}
