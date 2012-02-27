/*
 * PlinkIo.cpp
 *
 *  Created on: Feb 8, 2012
 *      Author: fmattias
 */

#include "PlinkIo.h"

PlinkIo::PlinkIo(const std::string &plinkPath)
{
	if( pio_open( &m_plinkFile, plinkPath.c_str( ) ) == PIO_OK )
	{
		if( pio_one_locus_per_row( &m_plinkFile ) )
		{
			m_isOpen = true;
		}
		else
		{
			m_isOpen = false;
			pio_close( &m_plinkFile );
		}
	}
	else
	{
		m_isOpen = false;
	}
}

ColumnData<unsigned char>
PlinkIo::getSnps()
{
	if( !m_isOpen )
	{
		return ColumnData<unsigned char>( );
	}

	pio_reset_row( &m_plinkFile );


	unsigned int numSamples = pio_num_samples( &m_plinkFile );
	ColumnData<unsigned char> allSnps;
	std::vector<unsigned char> snpRow( numSamples, 3 );

	while( pio_next_row( &m_plinkFile, &snpRow.front( ) ) == PIO_OK )
	{
		allSnps.addColumn( snpRow );
	}

	return allSnps;
}

std::vector<bool>
PlinkIo::getPhenotypes()
{
	if( !m_isOpen )
	{
		return std::vector<bool>( );
	}

	std::vector<bool> phenotypes( pio_num_samples( &m_plinkFile ) );
	for(unsigned int i = 0; i < pio_num_samples( &m_plinkFile ); i++)
	{
		struct pio_sample_t *sample = pio_get_sample( &m_plinkFile, i );
		if( sample->affection == PIO_CONTROL )
		{
			phenotypes[ i ] = false;
		}
		else if( sample->affection == PIO_CASE )
		{
			phenotypes[ i ] = true;
		}
	}

	return phenotypes;
}

std::string
PlinkIo::getLocus(unsigned int i)
{
	if( !m_isOpen )
	{
		return std::string( "" );
	}

	struct pio_locus_t *locus = pio_get_locus( &m_plinkFile, i );
	if( locus != NULL )
	{
		return std::string( locus->name );
	}
	else
	{
		return std::string( "" );
	}
}

void
PlinkIo::close()
{
	pio_close( &m_plinkFile );
}

PlinkIo::~PlinkIo()
{
	pio_close( &m_plinkFile );
}
