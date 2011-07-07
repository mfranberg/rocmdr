/*
 * ColumnHasher.cpp
 *
 *  Created on: Jul 7, 2011
 *      Author: fmattias
 */

#include <algorithm>
#include <stdexcept>
#include <vector>

#include <math.h>

#include <ColumnHasher.h>

void
ColumnHasher::hash(ColumnData<unsigned int> &data, std::vector<hash_type> *hashList)
{
	hashList->resize( data.numberOfRows( ), 0 );
	unsigned int totalNumberOfBits = 0;

	for(unsigned int i = 0; i < data.size( ); i++)
	{
		std::vector<unsigned int> column = data.getColumn( i );
		unsigned int bitsRequired = numBitsForColumn( column );
		totalNumberOfBits += bitsRequired;

		if( totalNumberOfBits > sizeof( hash_type )*8 )
		{
			throw std::runtime_error( "Hasher cannot handle this many bits." );
		}

		hashColumn( column, hashList, bitsRequired );
	}
}

void
ColumnHasher::hashColumn(const std::vector<unsigned int> &column, std::vector<hash_type> *hashList, unsigned int numBits)
{
	for(unsigned int i = 0; i < column.size( ); i++)
	{
		hash_type truncatedValue = column[ i ] & ((1 << numBits) - 1);
		hash_type currentHash = (*hashList)[ i ];
		hash_type newHash = (currentHash << numBits) | truncatedValue;

		(*hashList)[ i ] = newHash;
	}
}

unsigned int
ColumnHasher::numBitsForColumn(const std::vector<unsigned int> &column)
{
	unsigned int max = *std::max_element( column.begin( ), column.end( ) );
	return ceilf( log2f( (float) max ) );

}
