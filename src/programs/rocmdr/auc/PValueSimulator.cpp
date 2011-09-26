/*
 * PValueSimulator.cpp
 *
 *  Created on: Sep 22, 2011
 *      Author: fmattias
 */
#include <algorithm>
#include <functional>

#include <time.h>

#include <ColumnData.h>
#include <PhenotypeMapping.h>
#include <RocMdrAnalysis.h>

#include "PValueSimulator.h"

PValueSimulator::PValueSimulator(const std::vector<unsigned int> &columnRanges, unsigned int numSamples)
: m_columnRanges( columnRanges ),
  m_numSamples( numSamples )
{
	srandom( time( NULL ) );
}

float
PValueSimulator::computePValue(float auc)
{
	if( m_simulatedAuc.size( ) <= 0 )
	{
		for(unsigned int i = 0; i < SIMULATION_ITERATIONS; i++)
		{
			ColumnData<unsigned int> data( m_numSamples );
			generateRandomData( m_columnRanges, m_numSamples, &data );

			std::vector<bool> phenotypes;
			generatePhenotypes( m_numSamples, &phenotypes );

			PhenotypeMapping mapping( phenotypes );
			RocMdrAnalysis mdrAnalyzer( data, phenotypes );

			m_simulatedAuc.push_back( mdrAnalyzer.calculateAuc( ) );
		}
	}

	float numGreater = std::count_if( m_simulatedAuc.begin( ), m_simulatedAuc.end( ),
									  std::bind2nd( std::greater<float>( ), auc ) );
	return numGreater / m_simulatedAuc.size( );
}

void
PValueSimulator::generateRandomData(std::vector<unsigned int> columnRanges, unsigned int numSamples, ColumnData<unsigned int> *data)
{
	std::vector< unsigned int > column( numSamples, 0 );
	for(unsigned int i = 0; i < columnRanges.size( ); i++)
	{
		for(unsigned int j = 0; j < numSamples; j++)
		{
			column[ j ] = random( ) % columnRanges[ i ];
		}
		data->addColumn( column );
	}
}

void
PValueSimulator::generatePhenotypes(unsigned int numSamples, std::vector<bool> *phenotypes)
{
	phenotypes->resize( numSamples, false );
	for(unsigned int i = 0; i < numSamples; i++)
	{
		(*phenotypes)[ i ] = (random( ) / ((float) RAND_MAX)) <= 0.5;
	}
}

void
PValueSimulator::setSimulation(std::vector<float> simulation)
{
	m_simulatedAuc = simulation;
}

const std::vector<float> &
PValueSimulator::getSimulation()
{
	// Initialize simulation
	computePValue( 0.0 );

	return m_simulatedAuc;
}
