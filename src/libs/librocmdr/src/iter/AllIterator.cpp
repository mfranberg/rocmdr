/*
 * RocMdrBatch.cpp
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */

#include <util/SplitIndices.h>

#include <iter/AllIterator.h>

AllIterator::AllIterator(ColumnData<unsigned char> &snps, PhenotypeMapping &phenotypes, unsigned int interactionOrder)
:	m_snps( snps ),
 	m_phenotypes( phenotypes ),
 	m_interactionOrder( interactionOrder ),
 	m_index( 0 ),
 	m_numParts( 1 )
{

}

AllIterator::AllIterator(const AllIterator &other, size_t index, size_t numParts)
:	m_snps( other.m_snps ),
 	m_phenotypes( other.m_phenotypes ),
 	m_interactionOrder( other.m_interactionOrder ),
 	m_index( index ),
 	m_numParts( numParts )
{

}

AllIterator::AllIterator(const AllIterator &other)
: m_snps( other.m_snps ),
  m_phenotypes( other.m_phenotypes ),
  m_index( other.m_index ),
  m_numParts( other.m_numParts )
{

}

AllIterator &
AllIterator::operator=(const AllIterator &other)
{
	if( this != &other )
	{
		m_snps = other.m_snps;
		m_phenotypes = other.m_phenotypes;
		m_index = other.m_index;
		m_numParts = other.m_numParts;
	}

	return *this;
}

AllIterator::~AllIterator()
{
}

void
AllIterator::visitAll(InteractionVisitor *visitor)
{
	RecursionState state( m_interactionOrder, m_snps );
	visitAllRecursive( visitor, state );
}

void
AllIterator::visitAllRecursive(InteractionVisitor *visitor, RecursionState &state)
{
	if( state.done( ) )
	{
		Range range = splitIndices( state.getLastIndex( ) + 1, m_snps.size( ), m_index, m_numParts );
		for(size_t i = range.first; i < range.second; i++)
		{
			state.push( i );

			visitor->visit( state.getCurrentSnps( ), state.getCurrentIndices( ), m_phenotypes );

			state.pop( );
		}
	}
	else
	{
		for(unsigned int i = state.getLastIndex( ) + 1; i < m_snps.size( ); i++)
		{
			state.push( i );

			visitAllRecursive( visitor, state );

			state.pop( );
		}
	}
}

InteractionIterator *
AllIterator::partial(size_t index, size_t numParts)
{
	return new AllIterator( *this, index, numParts );
}
