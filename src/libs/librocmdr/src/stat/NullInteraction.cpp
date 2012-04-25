/*
 * NullInteraction.cpp
 *
 *  Created on: Apr 5, 2012
 *      Author: fmattias
 */
#include <algorithm>
#include <functional>
#include <vector>

#include <RocMdrAnalysis.h>
#include <data/algorithm.h>

#include <stat/NullInteraction.h>

NullInteraction::NullInteraction(unsigned int numIters)
: m_numIters( numIters )
{

}


float
NullInteraction::getPValue(float auc, ColumnData<unsigned char> &data, const PhenotypeMapping &phenotypes)
{
	std::vector<float> simulatedAuc;

	for(unsigned int i = 0; i < m_numIters; i++)
	{
		ColumnData<unsigned char> shuffledData = shuffleColumnData( data, phenotypes );

		RocMdrAnalysis rocMdr( shuffledData, phenotypes );
		simulatedAuc.push_back( rocMdr.getAuc( ) );
	}

	unsigned int numGreater = std::count_if( simulatedAuc.begin( ),
										     simulatedAuc.end( ),
										     std::bind2nd( std::greater_equal<float>( ), auc ) );

	return numGreater / ( (float) simulatedAuc.size( ) );
}
