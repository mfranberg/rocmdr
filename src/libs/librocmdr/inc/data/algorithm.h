/*
 * algorithm.h
 *
 *  Created on: Apr 4, 2012
 *      Author: fmattias
 */

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <algorithm>
#include <vector>

#include "ColumnData.h"

/**
 * Shuffles the data within each class, effectively removing the
 * conditional dependence.
 *
 * @param data The data to shuffle.
 * @param phenotypes Labels of the data.
 *
 * @return The shuffled data.
 */
template <class T>
std::vector<T> shuffleColumn(std::vector<T> data, const PhenotypeMapping &phenotypes)
{
	std::vector<unsigned int> positive = phenotypes.getPositiveIndices( );
	random_shuffle( positive.begin( ), positive.end( ) );

	for(unsigned int i = 1; i < positive.size( ); i++)
	{
		unsigned int first = positive[ i ];
		unsigned int second = positive[ i - 1 ];
		std::swap( data[ first ], data[ second ] );
	}

	std::vector<unsigned int> negative = phenotypes.getNegativeIndices( );
	random_shuffle( negative.begin( ), negative.end( ) );

	for(unsigned int i = 1; i < negative.size( ); i++)
	{
		unsigned int first = negative[ i ];
		unsigned int second = negative[ i - 1 ];
		std::swap( data[ first ], data[ second ] );
	}

	return data;
}

/**
 * Shuffles all columns individually in a ColumnData object.
 *
 * Note: The shuffle creates a new ColumnData object and returns it.
 *
 * @param data The data to shuffle.
 * @param phenotypes Phenotypes.
 *
 * @return The shuffled data.
 */
template <class T>
ColumnData<T> shuffleColumnData(ColumnData<T> data, const PhenotypeMapping &phenotypes)
{
	ColumnData<T> shuffledData;

	for(unsigned int i = 0; i < data.size( ); i++)
	{
		std::vector<T> shuffledColumn = shuffleColumn( data.getColumn( i ), phenotypes );
		shuffledData.addColumn( shuffledColumn );
	}

	return shuffledData;
}

#endif /* ALGORITHM_H_ */
