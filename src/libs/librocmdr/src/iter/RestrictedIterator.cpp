/*
 * RocMdrBatch.cpp
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */
#include <RocMdrAnalysis.h>

#include <iter/RestrictedIterator.h>
#include <util/SplitIndices.h>

RestrictedIterator::RestrictedIterator(ColumnData<unsigned char> &snps, PhenotypeMapping &phenotypes,
					  const std::vector<unsigned int> &restricted, unsigned int interactionOrder)
: m_snps( snps ),
  m_phenotypes( phenotypes ),
  m_restricted( restricted ),
  m_restrictedAsSet( restricted.begin( ), restricted.end( ) ),
  m_interactionOrder( interactionOrder ),
  m_index( 0 ),
  m_numParts( 1 )
{

}


RestrictedIterator::RestrictedIterator(const RestrictedIterator &other, size_t index, size_t numParts)
: m_snps( other.m_snps ),
  m_phenotypes( other.m_phenotypes ),
  m_restricted( other.m_restricted ),
  m_restrictedAsSet( other.m_restricted.begin( ), other.m_restricted.end( ) ),
  m_interactionOrder( other.m_interactionOrder ),
  m_index( index ),
  m_numParts( numParts )
{

}

RestrictedIterator::RestrictedIterator(const RestrictedIterator &other)
: m_snps( other.m_snps ),
  m_phenotypes( other.m_phenotypes ),
  m_restricted( other.m_restricted ),
  m_restrictedAsSet( other.m_restrictedAsSet ),
  m_interactionOrder( other.m_interactionOrder ),
  m_index( other.m_index ),
  m_numParts( other.m_numParts )
{

}

RestrictedIterator &
RestrictedIterator::operator=(const RestrictedIterator &other)
{
	if( this != &other )
	{
		m_snps = other.m_snps;
		m_phenotypes = other.m_phenotypes;
		m_restricted.assign( other.m_restricted.begin( ), other.m_restricted.end( ) );
		m_restrictedAsSet.clear( );
		m_restrictedAsSet.insert( other.m_restricted.begin( ), other.m_restricted.end( ) );
		m_index = other.m_index;
		m_numParts = other.m_numParts;
	}

	return *this;
}

void
RestrictedIterator::visitAll(InteractionVisitor *visitor)
{
	RecursionState state( m_interactionOrder + 1, m_snps );
	visitAllRecursive( visitor, state );

	for(size_t i = 0; i < m_restricted.size( ); i++)
	{
		state.push( m_restricted[ i ] );

		visitAllRecursive( visitor, state );

		state.pop( );
	}

}

unsigned int
RestrictedIterator::getStartIndex(RecursionState &state)
{
	if( state.depth( ) > 1 )
	{
		return state.getLastIndex( ) + 1;
	}
	else
	{
		return 0;
	}
}

void
RestrictedIterator::visitAllRecursive(InteractionVisitor *visitor, RecursionState &state)
{
	if( state.done( ) )
	{
		Range range = splitIndices( getStartIndex( state ), m_snps.size( ), m_index, m_numParts );
		for(size_t i = range.first; i < range.second; i++)
		{
			if( m_restrictedAsSet.count( i ) > 0 )
			{
				continue;
			}

			state.push( i );

			visitor->visit( state.getCurrentSnps( ), state.getCurrentIndices( ), m_phenotypes );

			state.pop( );
		}
	}
	else
	{
		for(unsigned int i = getStartIndex( state ); i < m_snps.size( ); i++)
		{
			if( m_restrictedAsSet.count( i ) > 0 )
			{
				continue;
			}

			state.push( i );

			visitAllRecursive( visitor, state );

			state.pop( );
		}
	}
}

InteractionIterator *
RestrictedIterator::partial(size_t index, size_t numParts)
{
	return new RestrictedIterator( *this, index, numParts );
}
