/*
 * SplitIndices.cpp
 *
 *  Created on: Aug 30, 2012
 *      Author: fmattias
 */
#include <algorithm>

#include <util/SplitIndices.h>

Range
splitIndices(size_t start, size_t end, size_t index, size_t numParts)
{
	unsigned int snpsPerPart = ( end - start + 1 ) / numParts;
	if( snpsPerPart == 0 )
	{
		snpsPerPart = 1;
	}

	Range range;
	range.first = std::min( start + index * snpsPerPart, end );
	range.second = std::min( range.first + snpsPerPart, end );

	return range;
}
