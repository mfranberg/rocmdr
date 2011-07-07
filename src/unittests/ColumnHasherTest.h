/*
 * ColumnHasherTest.h
 *
 *  Created on: Jul 7, 2011
 *      Author: fmattias
 */

#ifndef COLUMNHASHERTEST_H_
#define COLUMNHASHERTEST_H_

#include <vector>

#include <ColumnData.h>
#include <ColumnHasher.h>

#include <cxxtest/TestSuite.h>

/**
 * Number of rows.
 */
static const unsigned int COLUMN_SIZE = 6;

/**
 * Number of columns.
 */
static const unsigned int NUM_COLUMNS = 2;

/**
 * Two columns for testing that two equal rows get
 * the same hash value.
 */
unsigned int g_columns[NUM_COLUMNS][COLUMN_SIZE] = { {1,4,2,3,4,4}, {3,2,4,3,1,2} };

/**
 * The two row indices that should have the same hash value.
 */
static const unsigned int EQUAL_INDEX_FIRST = 1;
static const unsigned int EQUAL_INDEX_SECOND = 5;

class ColumnDataTestSuite : public CxxTest::TestSuite
{
public:
	/**
	 * Tests that the hash of a single value comes back
	 * as itself.
	 */
	void testSingleHash()
	{
		std::vector<unsigned int> column( 1,3 );
		ColumnData<unsigned int> data;
		data.addColumn( column );

		std::vector<ColumnHasher::hash_type> hashList;
		ColumnHasher::hash( data, &hashList );

		TS_ASSERT_EQUALS( hashList.size( ), 1u );
		TS_ASSERT_EQUALS( hashList[ 0 ], 3u );
	}

	/**
	 * Tests that two of the rows that are equal also get
	 * the same hash value.
	 */
	void testTwoEqualHashes()
	{
		ColumnData<unsigned int> columns;
		for(unsigned int i = 0; i < NUM_COLUMNS; i++)
		{
			unsigned int *columnData = g_columns[ i ];
			std::vector<unsigned int> column( columnData, columnData + COLUMN_SIZE );
			columns.addColumn( column );
		}

		std::vector<ColumnHasher::hash_type> hashList;
		ColumnHasher::hash( columns, &hashList );

		TS_ASSERT_EQUALS( hashList.size( ), COLUMN_SIZE );
		TS_ASSERT_EQUALS( hashList[ EQUAL_INDEX_FIRST ], hashList[ EQUAL_INDEX_SECOND ] );
	}
};

#endif /* COLUMNHASHERTEST_H_ */
