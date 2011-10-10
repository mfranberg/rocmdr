/*
 * NoAssociationSimulator.cpp
 *
 *  Created on: Oct 7, 2011
 *      Author: fmattias
 */
#include <algorithm>
#include <functional>

#include <time.h>

#include <ColumnData.h>
#include <PhenotypeMapping.h>
#include <RocMdrAnalysis.h>

#include "NoAssociationSimulator.h"

NoAssociationSimulator::NoAssociationSimulator(ColumnData<unsigned int> &factors, const PhenotypeMapping &phenotypes)
: m_factors( factors ),
  m_phenotypes( phenotypes ),
  m_numSamples( SIMULATION_ITERATIONS )
{
	srandom( time( NULL ) );
}

float
NoAssociationSimulator::computePValue(float auc)
{
	if( m_simulatedAuc.size( ) != m_numSamples )
	{
		runSimulation( );
	}

	float numGreater = std::count_if( m_simulatedAuc.begin( ), m_simulatedAuc.end( ),
									  std::bind2nd( std::greater<float>( ), auc ) );
	return numGreater / m_simulatedAuc.size( );
}

void
NoAssociationSimulator::runSimulation()
{
	m_simulatedAuc.resize( m_numSamples, 0.0f );

	std::vector<unsigned int> snps( m_phenotypes.size( ) );
	for(unsigned int sample = 0; sample < m_numSamples; sample++)
	{
		generateSnp( m_phenotypes.size( ), &snps );

		m_factors.addColumn( snps );

		RocMdrAnalysis mdrAnalyzer( m_factors, m_phenotypes );
		m_simulatedAuc[ sample ] = mdrAnalyzer.calculateAuc( );

		m_factors.removeColumnLast( );
	}
}

void
NoAssociationSimulator::setNumberOfSamples(unsigned int numSamples)
{
	m_numSamples = numSamples;
}

void
NoAssociationSimulator::generateSnp(unsigned int numSnps, std::vector<unsigned int> *simulatedSnp)
{
	if( simulatedSnp->size( ) != numSnps )
	{
		simulatedSnp->resize( numSnps, 0 );
	}

	for(unsigned int i = 0; i < numSnps; i++)
	{
		(*simulatedSnp)[ i ] = random( ) % 3;
	}
}
