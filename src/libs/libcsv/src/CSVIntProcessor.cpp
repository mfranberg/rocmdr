/*
 * IntProcessor.cpp
 *
 *  Created on: Sep 20, 2011
 *      Author: fmattias
 */
#include <stdexcept>

#include "CSVIntProcessor.h"

CSVIntProcessor::CSVIntProcessor()
: m_row( 0 ),
  m_column( 0 )
{

}

CSVIntProcessor::~CSVIntProcessor()
{

}

void
CSVIntProcessor::onField(const std::string &field)
{
	int fieldAsInt;
	unsigned int charactersUsed;
	int numScannedElements = sscanf( field.c_str( ), "%d%n", &fieldAsInt, &charactersUsed );
	if( numScannedElements == 1 && charactersUsed == field.size( ) )
	{
		m_currentRow.push_back( fieldAsInt );
	}
	else
	{
		if( m_row == 0 )
		{
			m_columnHeader.push_back( field );
		}
		else
		{
			throw std::runtime_error( "CSVIntProcessor::onField: Non integer field found." );
		}
	}

	m_column++;
}

void
CSVIntProcessor::onRowEnd()
{
	m_row++;
	m_column = 0;

	if( m_currentRow.size( ) > 0 && m_columns.size( ) == 0 )
	{
		m_columns.resize( m_currentRow.size( ) );
	}

	if( m_columns.size( ) != m_currentRow.size( ) )
	{
		throw std::runtime_error( "CSVIntProcessor::onRowEnd: Cannot have two rows with a different number of columns." );
	}

	for(unsigned int i = 0; i < m_currentRow.size( ); i++)
	{
		m_columns[ i ].push_back( m_currentRow[ i ] );
	}
	m_currentRow.clear( );
}

std::vector< std::vector< int > >
CSVIntProcessor::getColumnData()
{
	return m_columns;
}

std::vector< std::string >
CSVIntProcessor::getHeader()
{
	return m_columnHeader;
}
