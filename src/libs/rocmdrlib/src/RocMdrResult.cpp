/*
 * RocMdrResult.cpp
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */

#include <RocMdrResult.h>

RocMdrResult::RocMdrResult(std::vector<unsigned int> loci, float auc, float pValue)
: m_loci( loci ),
  m_auc( auc ),
  m_pValue( pValue )
{

}


RocMdrResult::RocMdrResult(const RocMdrResult &other)
: m_loci( other.m_loci ),
  m_auc( other.m_auc ),
  m_pValue( other.m_pValue )
{

}


RocMdrResult & RocMdrResult::operator=(const RocMdrResult &other)
{
	if( this != &other )
	{
		m_loci.clear( );
		m_loci.assign( other.m_loci.begin( ), other.m_loci.end( ) );

		m_auc = other.m_auc;
		m_pValue = other.m_pValue;
	}

	return *this;
}

void
RocMdrResult::addLocus(unsigned int locus)
{
	m_loci.push_back( locus );
}

void
RocMdrResult::removeLastLocus()
{
	m_loci.pop_back( );
}

std::vector<unsigned int>
RocMdrResult::getLoci() const
{
	return m_loci;
}

void
RocMdrResult::setResult(float auc, float pValue)
{
	m_auc = auc;
	m_pValue = pValue;
}

double
RocMdrResult::getPValue() const
{
	return m_pValue;
}

float
RocMdrResult::getAuc() const
{
	return m_auc;
}
