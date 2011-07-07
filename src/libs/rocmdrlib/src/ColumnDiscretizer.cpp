/*
 * ColumnDiscretizer.cpp
 *
 *  Created on: Jul 6, 2011
 *      Author: fmattias
 */

/*
 * ColumnDiscretizer.h
 *
 *  Created on: Jul 6, 2011
 *      Author: fmattias
 */

#include <algorithm>

#include <math.h>

#include <ColumnDiscretizer.h>


ColumnDiscretizer::ColumnDiscretizer(unsigned int numIntervals)
: m_numIntervals( numIntervals )
{

}

void ColumnDiscretizer::transform(ColumnData<float> &before, ColumnData<unsigned int> *after)
{
	for(unsigned int i = 0; i < before.size( ); i++)
	{
		std::vector<unsigned int> discretizedColumn( before.numberOfRows( ), 0 );
		discretizeColumn( before.getColumn( i ), m_numIntervals, &discretizedColumn );
		after->addColumn( discretizedColumn );
	}
}


void ColumnDiscretizer::discretizeColumn(const std::vector<float> &column,
					unsigned int numIntervals,
					std::vector<unsigned int> *discretizedColumn)
{
	if( discretizedColumn->size( ) != column.size( ) )
	{
		discretizedColumn->resize( column.size( ), 0 );
	}

	float minElement = *std::min_element( column.begin( ), column.end( ) );
	float maxElement = *std::max_element( column.begin( ), column.end( ) );

	float intervalSize = (maxElement - minElement) / (float)numIntervals;

	// This offset is added to make sure that we always start at 0
	float offset = -minElement;

	for(unsigned int i = 0; i < column.size( ); i ++)
	{
		if( column[ i ] != maxElement )
		{
			(*discretizedColumn)[ i ] = ((offset + column[ i ]) / intervalSize);
		}
		else
		{
			// Make sure the maximum element is in the last interval
			(*discretizedColumn)[ i ] = numIntervals - 1;
		}
	}
}


