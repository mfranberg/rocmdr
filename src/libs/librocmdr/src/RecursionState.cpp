/*
 * RecursionState.cpp
 *
 *  Created on: Feb 28, 2012
 *      Author: fmattias
 */
#include <RecursionState.h>

RecursionState::RecursionState(unsigned int maxDepth, ColumnData<unsigned char> &snps)
: m_maxDepth( maxDepth ),
  m_snps( snps )
{

}

RecursionState::RecursionState(const RecursionState &other)
: m_maxDepth( other.m_maxDepth ),
  m_snps( other.m_snps ),
  m_curSnps( other.m_curSnps ),
  m_curIndex( other.m_curIndex ),
  m_lastIndex( other.m_lastIndex )
{

}

RecursionState &
RecursionState::operator=(const RecursionState &other)
{
	if( this != &other )
	{
		m_maxDepth = other.m_maxDepth;
		m_snps = other.m_snps;
		m_curSnps = other.m_curSnps;
		m_curIndex.assign( other.m_curIndex.begin( ), other.m_curIndex.end( ) );
		m_lastIndex.assign( other.m_lastIndex.begin( ), other.m_lastIndex.end( ) );
	}

	return *this;
}

void
RecursionState::push(unsigned int index)
{
	m_curSnps.addColumn( m_snps.getColumn( index ) );
	m_curIndex.push_back( index );
	m_lastIndex.push_back( index );
}

void
RecursionState::push(unsigned int index, unsigned int virtualIndex)
{
	push( index );
	m_lastIndex.back( ) = virtualIndex;
}

void
RecursionState::pop()
{
	m_curSnps.removeColumnLast( );
	m_curIndex.pop_back( );
	m_lastIndex.pop_back( );
}

ColumnData<unsigned char> &
RecursionState::getCurrentSnps()
{
	return m_curSnps;
}

std::vector<unsigned int> &
RecursionState::getCurrentIndices()
{
	return m_curIndex;
}

unsigned int
RecursionState::nextIndex()
{
	if( m_lastIndex.size( ) == 0 )
	{
		return 0;
	}
	else
	{
		return m_lastIndex.back( ) + 1;
	}
}

bool
RecursionState::done()
{
	return m_curSnps.size( ) >= m_maxDepth - 1;
}
