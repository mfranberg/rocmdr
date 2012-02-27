/*
 * PhenotypeMapping.cpp
 *
 *  Created on: Jul 8, 2011
 *      Author: fmattias
 */

#include <algorithm>

#include <data/PhenotypeMapping.h>

PhenotypeMapping::PhenotypeMapping()
{
}

PhenotypeMapping::PhenotypeMapping(const std::vector<bool> &phenotypes)
: m_phenotypes( phenotypes )
{

}

PhenotypeMapping::PhenotypeMapping(const PhenotypeMapping &other)
: m_phenotypes( other.m_phenotypes )
{

}

PhenotypeMapping &
PhenotypeMapping::operator=(const PhenotypeMapping &other)
{
	if( this != &other )
	{
		m_phenotypes.clear( );
		m_phenotypes.assign( other.m_phenotypes.begin( ), other.m_phenotypes.end( ) );
	}

	return *this;
}

unsigned int
PhenotypeMapping::getNumPositive() const
{
	return count( m_phenotypes.begin( ), m_phenotypes.end( ), true );
}

unsigned int
PhenotypeMapping::getNumNegative() const
{
	return count( m_phenotypes.begin( ), m_phenotypes.end( ), false );
}

const std::vector<bool> &
PhenotypeMapping::getPhenotypes() const
{
	return m_phenotypes;
}

bool
PhenotypeMapping::isPositive(unsigned int index) const
{
	return m_phenotypes[ index ];
}

unsigned int
PhenotypeMapping::size() const
{
	return m_phenotypes.size( );
}
