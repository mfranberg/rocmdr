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
  m_curIndex( other.m_curIndex )
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
	}

	return *this;
}

void
RecursionState::push(unsigned int index)
{
	m_curSnps.addColumn( m_snps.getColumn( index ) );
	m_curIndex.push_back( index );
}

void
RecursionState::pop()
{
	m_curSnps.removeColumnLast( );
	m_curIndex.pop_back( );
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
RecursionState::getLastIndex()
{
	if( m_curIndex.size( ) == 0 )
	{
		return -1;
	}
	else
	{
		return m_curIndex.back( );
	}
}

unsigned int
RecursionState::depth()
{
	return m_curSnps.size( );
}

bool
RecursionState::done()
{
	return m_curSnps.size( ) >= m_maxDepth - 1;
}
