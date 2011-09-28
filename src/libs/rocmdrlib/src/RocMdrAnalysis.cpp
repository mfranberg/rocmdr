/*
 * RocMdrAnalysis.cpp
 *
 *  Created on: Jul 7, 2011
 *      Author: fmattias
 */

#include <algorithm>

#include <ColumnHasher.h>

#include <RocMdrAnalysis.h>

/**
 * This class is used to sort the cells. The cells
 * are sorted according to the their slope, but there
 * are some cases when the slope is infinite, which is
 * handled by just comparing the number of true positives.
 */
class CellComparator
{
public:
	/**
	 * Constructor.
	 *
	 * @param numPositives Number of positive samples.
	 * @param numNegatives Number of negative samples.
	 */
	CellComparator(unsigned int numPositives, unsigned int numNegatives)
	: m_numPositives( numPositives ),
	  m_numNegatives( numNegatives )
	{

	}

	bool operator()(const RocMdrAnalysis::CellInfo &a, const RocMdrAnalysis::CellInfo& b) const
	{
		if( a.numFp != 0 && b.numFp != 0 )
		{
			return computeSlope( a ) < computeSlope( b );
		}
		else if( a.numFp == 0 && b.numFp != 0 )
		{
			// Cell with 0 fp < Cell with more than 0 fp
			return false;
		}
		else if( a.numFp != 0 && b.numFp == 0 )
		{
			// Cell with more than 0 fp < Cell with 0 fp
			return true;
		}
		else
		{
			// Cell with 0 fp < Cell with 0 fp
			return a.numTp < b.numTp;
		}
	}

private:
	/**
	 * Computes the tpr / fpr slope at a given point.
	 */
	float computeSlope(const RocMdrAnalysis::CellInfo &cell) const
	{
		return (cell.numTp / m_numPositives) / (cell.numFp / m_numNegatives);
	}

	/**
	 * Number of positive samples.
	 */
	float m_numPositives;

	/**
	 * Number of negative samples.
	 */
	float m_numNegatives;
};

RocMdrAnalysis::RocMdrAnalysis(ColumnData<unsigned int> &data, const PhenotypeMapping &phenotypes)
: m_data( data ),
  m_phenotypes( phenotypes ),
  m_cellCounter( phenotypes.size( ) )
{
	m_cellCounter.set_empty_key( ULONG_LONG_MAX );
}

float
RocMdrAnalysis::calculateAuc()
{
	performAnalysis( );

	std::vector<CellInfo> cellList;
	sortCells( m_cellCounter, &cellList );

	int numPositive = m_phenotypes.getNumPositive( );
	int numNegative = m_phenotypes.getNumNegative( );

	int lastTp = 0;
	int lastFp = 0;
	float totalAuc = 0.0f;
	for(unsigned int i = 0; i < cellList.size( ); i++)
	{
		CellInfo *cellInfo = &cellList[ i ];
		totalAuc += cellInfo->numFp * (cellInfo->numTp + 2 * lastTp) / (float) (numPositive * numNegative);

		lastTp += cellInfo->numTp;
		lastFp += cellInfo->numFp;
	}

	return totalAuc * 0.5f * 1.0;
}

void
RocMdrAnalysis::getCombinations(std::vector< std::vector<unsigned int> > *combinationList)
{
	performAnalysis( );

	std::vector<CellInfo> cellList;
	sortCells( m_cellCounter, &cellList );

	unsigned int numColumns = m_data.size( );
	combinationList->resize( cellList.size( ) );
	for(unsigned int i = 0; i < cellList.size( ); i++)
	{
		for(unsigned int j = 0; j < numColumns; j++)
		{
			const std::vector<unsigned int> &column = m_data.getColumn( j );
			unsigned int value = column[ cellList[ i ].lastIndividual ];
			(*combinationList)[ i ].push_back( value );
		}
	}
}

void
RocMdrAnalysis::performAnalysis()
{
	if( m_cellCounter.size( ) > 0 )
	{
		// Analysis has already been performed.
		return;
	}

	hashSamples( m_data, &m_hashList );
	countSamplesInEachCell( m_phenotypes, m_hashList, &m_cellCounter );
}

void
RocMdrAnalysis::hashSamples(ColumnData<unsigned int> &data, std::vector<ColumnHasher::hash_type> *hashList)
{
	ColumnHasher::hash( data, hashList );
}

void
RocMdrAnalysis::countSamplesInEachCell(const PhenotypeMapping &phenotypes,
							  const std::vector<ColumnHasher::hash_type> &hashList,
							  google::dense_hash_map<ColumnHasher::hash_type, CellInfo> *cellCounter)
{
	for(unsigned int i = 0; i < hashList.size( ); i++)
	{
		ColumnHasher::hash_type cellHash = hashList[ i ];
		CellInfo *cellInfo = &(*cellCounter)[ cellHash ];
		if( m_phenotypes.isPositive( i ) )
		{
			cellInfo->numTp += 1;
		}
		else
		{
			cellInfo->numFp += 1;
		}
		cellInfo->lastIndividual = i;
	}
}

void
RocMdrAnalysis::sortCells(const google::dense_hash_map<ColumnHasher::hash_type, CellInfo> &cellCounter, std::vector<CellInfo> *cellList)
{
	google::dense_hash_map<ColumnHasher::hash_type, CellInfo>::const_iterator it;
	for(it = cellCounter.begin( ); it != cellCounter.end( ); ++it)
	{
		cellList->push_back( it->second );
	}
	CellComparator comparator( m_phenotypes.getNumPositive( ), m_phenotypes.getNumNegative( ) );
	sort( cellList->rbegin( ), cellList->rend( ), comparator );
}
