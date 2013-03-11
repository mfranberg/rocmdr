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
#include <tr1/random>

#include <data/ColumnData.h>

/**
 * A functor that generates a random integer in the
 * given range using a Mersenne twister random
 * generator. I use this instead of rand(), to avoid
 * the lock used in rand() that causes severe contention
 * when using multiple threads.
 */
class RandomInt
{
public:
	RandomInt(unsigned long seed)
	:	m_rand( ),
		m_generator( seed )
	{

	}

	/**
	 * Generates a random number between [0, max), i.e.
	 * non inclusively.
	 */
	int operator()(unsigned int max)
	{
		return m_rand( m_generator, max );
	}

private:
	/**
	 * Takes a uniform random integer in the given range.
	 */
	std::tr1::uniform_int<int> m_rand;

	/**
	 * Mersenne twister random generator.
	 */
	std::tr1::mt19937 m_generator;
};


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
std::vector<T> shuffleColumn(std::vector<T> data, const PhenotypeMapping &phenotypes, RandomInt &rand)
{
	std::vector<unsigned int> positive = phenotypes.getPositiveIndices( );
	for(unsigned int i = positive.size( ) - 1; i > 0; i--)
	{
		unsigned int j = rand( i );
		std::swap( data[ positive[ i ] ], data[ positive[ j ] ] );
	}

	std::vector<unsigned int> negative = phenotypes.getNegativeIndices( );
	for(unsigned int i = negative.size( ) - 1; i > 0; i--)
	{
		unsigned int j = rand( i );
		std::swap( data[ negative[ i ] ], data[ negative[ j ] ] );
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
ColumnData<T> shuffleColumnData(ColumnData<T> &data, const PhenotypeMapping &phenotypes)
{
	ColumnData<T> shuffledData;
	RandomInt rand = RandomInt( random( ) );

	for(unsigned int i = 0; i < data.size( ); i++)
	{
		std::vector<T> shuffledColumn = shuffleColumn( data.getColumn( i ), phenotypes, rand );
		shuffledData.addColumn( shuffledColumn );
	}

	return shuffledData;
}

#endif /* ALGORITHM_H_ */
