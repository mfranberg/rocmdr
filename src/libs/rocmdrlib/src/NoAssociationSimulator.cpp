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

NoAssociationSimulator::NoAssociationSimulator()
: m_numSamples( 0 ),
  m_order( 1 )
{
	srandom( time( NULL ) );
}

NoAssociationSimulator::NoAssociationSimulator(ColumnData<unsigned char> &snps, const std::vector<bool> &phenotypes)
: m_snps( snps ),
  m_phenotypes( phenotypes ),
  m_numSamples( 0 ),
  m_order( 1 )
{
	srandom( time( NULL ) );
}

void
NoAssociationSimulator::setData(ColumnData<unsigned char> &snps, const std::vector<bool> &phenotypes)
{
	m_snps = snps;
	m_phenotypes = phenotypes;
	m_simulatedAuc.clear( );
}

void
NoAssociationSimulator::setOrder(unsigned int order)
{
	m_order = order;
}

float
NoAssociationSimulator::computePValue(float auc)
{
	if( m_numSamples == 0 || ( m_simulatedAuc.size( ) != m_numSamples ) )
	{
		runSimulation( );
	}

	std::vector<float>::iterator firstGreaterElement;
	firstGreaterElement = std::upper_bound( m_simulatedAuc.begin( ), m_simulatedAuc.end( ), auc );
	unsigned int numGreater = m_simulatedAuc.size( ) - ( firstGreaterElement - m_simulatedAuc.begin( ) );

	return numGreater / ( (float) m_simulatedAuc.size( ) );
}

void
NoAssociationSimulator::runSimulation()
{
	if( m_numSamples == 0 )
	{
		m_numSamples = numCombinations( m_order, m_snps.size( ) );
	}

	m_simulatedAuc.resize( m_numSamples, 0.0f );

	std::vector<bool> shuffledPhenotypes( m_phenotypes );
	for(unsigned int sample = 0; sample < m_numSamples; sample++)
	{
		shufflePhenotypes( &shuffledPhenotypes );

		ColumnData<unsigned char> snps;
		for(unsigned int k = 0; k < m_order; k++)
		{
			unsigned int snpIndex = getRandomInteger( m_snps.size( ) - 1 );
			snps.addColumn( m_snps.getColumn( snpIndex ) );
		}

		RocMdrAnalysis mdrAnalyzer( snps, m_phenotypes );
		m_simulatedAuc[ sample ] = mdrAnalyzer.getAuc( );
	}

	sort( m_simulatedAuc.begin( ), m_simulatedAuc.end( ) );
}

void
NoAssociationSimulator::setNumberOfSamples(unsigned int numSamples)
{
	m_numSamples = numSamples;
}

void
NoAssociationSimulator::shufflePhenotypes(std::vector<bool> *phenotypes)
{
	/* Fisher-Yates shuffle */
	for(unsigned int i = phenotypes->size( ); i >= 1; i--)
	{
		/* j random between 0 and i inclusive. */
		unsigned int j = getRandomInteger( i );

		bool tmp = (*phenotypes)[ i ];
		(*phenotypes)[ i ] = (*phenotypes)[ j ];
		(*phenotypes)[ j ] = tmp;
	}
}

unsigned int
NoAssociationSimulator::getRandomInteger(unsigned int upper)
{
	double r = random( ) / ( ( (double) RAND_MAX ) + 1 );

	return (unsigned int) ( r * ( upper + 1 ) );
}

unsigned int
NoAssociationSimulator::numCombinations(unsigned int interactionOrder, unsigned int numSnps)
{
	int numCombinations = 1;
	for(unsigned int i = 1; i <= interactionOrder; i++)
	{
		numCombinations *= ( numSnps - ( i - 1 ) );
	}

	return numCombinations;
}
