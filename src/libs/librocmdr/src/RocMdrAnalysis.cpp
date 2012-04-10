/*
 * RocMdrAnalysis.cpp
 *
 *  Created on: Jul 7, 2011
 *      Author: fmattias
 */

#include <algorithm>

#include <Cell.h>
#include <CellComparator.h>
#include <data/ColumnHasher.h>
#include <RocMdrAnalysis.h>

#include <climits>
#include <cmath>
#include <util/normal.h>

RocMdrAnalysis::RocMdrAnalysis(ColumnData<RocMdrAnalysis::column_type> &data, const PhenotypeMapping &phenotypes)
: m_data( data ),
  m_phenotypes( phenotypes )
{
	assert( data.numberOfRows( ) == phenotypes.size( ) );
}

float
RocMdrAnalysis::getAuc()
{
	sortCells( );

	int numPositive = m_phenotypes.getNumPositive( );
	int numNegative = m_phenotypes.getNumNegative( );

	int lastTp = 0;
	int lastFp = 0;
	float totalAuc = 0.0f;
	for(unsigned int i = 0; i < m_sortedCells.size( ); i++)
	{
		Cell *cell = &m_sortedCells[ i ];
		totalAuc += cell->numFp * ( cell->numTp + 2 * lastTp ) / (float) ( numPositive * numNegative );

		lastTp += cell->numTp;
		lastFp += cell->numFp;
	}

	return totalAuc * 0.5f * 1.0;
}

double
RocMdrAnalysis::getPValue()
{
	sortCells( );

	double numPositive = 0;
	double numNegative = 0;

	double tieCorrection = 0;
	for(unsigned int i = 0; i < m_sortedCells.size( ); i++)
	{
		Cell *cell = &m_sortedCells[ i ];
		unsigned int numTies = cell->numTp + cell->numFp;
		numPositive += cell->numTp;
		numNegative += cell->numFp;

		if( numTies > 0 )
		{
			tieCorrection -= ( numTies * numTies * numTies - numTies ) / 12.0;
		}
	}

	double N = numPositive + numNegative;
	tieCorrection += ( N * N * N - N ) / 12.0;

	double mean = numPositive * numNegative / 2;
	double sigma = std::sqrt( ( numPositive * numNegative / ( N * ( N - 1 ) ) ) * tieCorrection );
	if( std::abs( tieCorrection ) <= 1e-6 )
	{
		sigma = std::sqrt( numPositive * numNegative * ( N + 1 ) / 12.0 );
	}

	double U = getAuc( ) * numPositive * numNegative;
	double uStdNormal = ( U - mean ) / sigma;
	double pValue = 2 * normcdf( -uStdNormal );
	return pValue;
}

void
RocMdrAnalysis::sortCells()
{
	if( m_sortedCells.size( ) > 0 )
	{
		return;
	}

	std::vector<ColumnHasher::hash_type> hashList;
	hashSamples( m_data, &hashList );

	google::dense_hash_map<ColumnHasher::hash_type, Cell> cellCounter( m_phenotypes.size( ) );
	countSamplesInEachCell( m_phenotypes, hashList, &cellCounter );

	/* Add and sort cells */
	google::dense_hash_map<ColumnHasher::hash_type, Cell>::const_iterator it;
	for(it = cellCounter.begin( ); it != cellCounter.end( ); ++it)
	{
		m_sortedCells.push_back( it->second );
	}

	CellComparator comparator( m_phenotypes.getNumPositive( ), m_phenotypes.getNumNegative( ) );
	sort( m_sortedCells.rbegin( ), m_sortedCells.rend( ), comparator );
}

void
RocMdrAnalysis::hashSamples(ColumnData<RocMdrAnalysis::column_type> &data, std::vector<ColumnHasher::hash_type> *hashList)
{
	ColumnHasher::hash( data, hashList );
}

void
RocMdrAnalysis::countSamplesInEachCell(const PhenotypeMapping &phenotypes,
							  const std::vector<ColumnHasher::hash_type> &hashList,
							  google::dense_hash_map<ColumnHasher::hash_type, Cell> *cellCounter)
{
	cellCounter->set_empty_key( ULONG_MAX );

	for(unsigned int i = 0; i < hashList.size( ); i++)
	{
		ColumnHasher::hash_type cellHash = hashList[ i ];

		Cell &cell = (*cellCounter)[ cellHash ];
		if( m_phenotypes.isPositive( i ) )
		{
			cell.numTp += 1;
		}
		else
		{
			cell.numFp += 1;
		}
		cell.lastIndividual = i;
	}
}
