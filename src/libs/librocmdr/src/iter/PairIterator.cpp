/*
 * RocMdrPairBatch.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: fmattias
 */
#include <util/SplitIndices.h>

#include <iter/PairIterator.h>

PairIterator::PairIterator(ColumnData<unsigned char> &snps,
								 PhenotypeMapping &phenotypes,
								 const std::vector<std::pair<unsigned int, unsigned int> > &pairs)
: m_snps( snps ),
  m_phenotypes( phenotypes ),
  m_pairs( pairs ),
  m_index( 0 ),
  m_numParts( 1 )
{

}

PairIterator::PairIterator(const PairIterator &other, size_t index, size_t numParts)
: m_snps( other.m_snps ),
  m_phenotypes( other.m_phenotypes ),
  m_pairs( other.m_pairs ),
  m_index( index ),
  m_numParts( numParts )
{

}

PairIterator::PairIterator(const PairIterator &other)
: m_snps( other.m_snps ),
  m_phenotypes( other.m_phenotypes ),
  m_pairs( other.m_pairs ),
  m_index( other.m_index ),
  m_numParts( other.m_numParts )
{

}

PairIterator &
PairIterator::operator=(const PairIterator &other)
{
	if( this != &other )
	{
		m_snps = other.m_snps;
		m_phenotypes = other.m_phenotypes;
		m_pairs.assign( other.m_pairs.begin( ), other.m_pairs.end( ) );
		m_index = other.m_index;
		m_numParts = other.m_numParts;
	}

	return *this;
}

void
PairIterator::visitAll(InteractionVisitor *visitor)
{
	Range range = splitIndices( 0, m_pairs.size( ), m_index, m_numParts );

	for(unsigned int i = range.first; i < range.second; i++)
	{
		std::pair<unsigned int, unsigned int> locusPair = m_pairs[ i ];

		ColumnData<unsigned char> snps;
		snps.addColumn( m_snps.getColumn( locusPair.first ) );
		snps.addColumn( m_snps.getColumn( locusPair.second ) );

		std::vector<unsigned int> curIndicies;
		curIndicies.push_back( locusPair.first );
		curIndicies.push_back( locusPair.second );

		visitor->visit( snps, curIndicies, m_phenotypes );
	}
}


InteractionIterator *
PairIterator::partial(size_t index, size_t numParts)
{
	return new PairIterator( *this, index, numParts );
}
