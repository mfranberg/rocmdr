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
	for(unsigned int i = 0; i < phenotypes.size( ); i++)
	{
		if( phenotypes[ i ] == true )
		{
			m_positive.push_back( i );
		}
		else
		{
			m_negative.push_back( i );
		}
	}

}

PhenotypeMapping::PhenotypeMapping(const PhenotypeMapping &other)
: m_phenotypes( other.m_phenotypes ),
  m_positive( other.m_positive ),
  m_negative( other.m_negative )
{

}

PhenotypeMapping &
PhenotypeMapping::operator=(const PhenotypeMapping &other)
{
	if( this != &other )
	{
		m_phenotypes.clear( );
		m_positive.clear( );
		m_negative.clear( );

		m_phenotypes.assign( other.m_phenotypes.begin( ), other.m_phenotypes.end( ) );
		m_positive.assign( other.m_positive.begin( ), other.m_positive.end( ) );
		m_negative.assign( other.m_negative.begin( ), other.m_negative.end( ) );
	}

	return *this;
}

unsigned int
PhenotypeMapping::getNumPositive() const
{
	return m_positive.size( );
}

unsigned int
PhenotypeMapping::getNumNegative() const
{
	return m_negative.size( );
}

std::vector<unsigned int>
PhenotypeMapping::getPositiveIndices() const
{
	return m_positive;
}

std::vector<unsigned int>
PhenotypeMapping::getNegativeIndices() const
{
	return m_negative;
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
