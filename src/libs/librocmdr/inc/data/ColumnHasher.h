/*
 * ColumnHasher.h
 *
 *  Created on: Jul 7, 2011
 *      Author: fmattias
 */

#ifndef COLUMNHASHER_H_
#define COLUMNHASHER_H_

#include <vector>

#include <data/ColumnData.h>

/**
 * Class that collapses a set of columns into
 * a single column, such that collapse is unique.
 *
 * It does this by transforming each row into a
 * binary string.
 *
 * TODO: Maybe this should be a ColumnTransformer instead.
 */
class ColumnHasher
{
public:
	/**
	 * Type used for columns.
	 */
	typedef unsigned char column_type;

	/**
	 * The type used for the hashed values. You can always
	 * assume that this will support the operators < and =.
	 */
	typedef unsigned long hash_type;

	/**
	 * Hashes the given set of columns, such that each row
	 * is transformed into a unique hash value.
	 *
	 * @param data The columns that will be transformed.
	 * @param hashList The generated hash values will be stored here.
	 */
	static void hash(ColumnData<column_type> &data, std::vector<hash_type> *hashList);

private:
	/**
	 * Hashes a single column and puts the results in given
	 * hash list.
	 *
	 * @param column The column to hash.
	 * @param hashList The output hashes will be stored here.
	 * @param numBits The number of bits each column value will be trunacted
	 *                and hashed to.
	 */
	static void hashColumn(const std::vector<column_type> &column, std::vector<hash_type> *hashList, unsigned int numBits);

	/**
	 * Returns the number of bits required to hash the given column.
	 *
	 * @param column The column to find the number of bits required.
	 */
	static unsigned int numBitsForColumn(const std::vector<column_type> &column);
};


#endif /* COLUMNHASHER_H_ */
