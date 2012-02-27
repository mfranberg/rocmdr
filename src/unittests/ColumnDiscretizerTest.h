/*
 * ColumnDiscretizerTest.h
 *
 *  Created on: Jul 6, 2011
 *      Author: fmattias
 */

#ifndef COLUMNDISCRETIZERTEST_H_
#define COLUMNDISCRETIZERTEST_H_

#include <vector>

#include <ColumnData.h>
#include <ColumnDiscretizer.h>

#include <cxxtest/TestSuite.h>

/**
 * Number of test columns.
 */
static const unsigned int NUM_COLUMNS = 2;

/**
 * Length of each test column.
 */
static const unsigned int COLUMN_SIZE = 4;

/**
 * Number of intervals that each column will be divided
 * into.
 */
static const unsigned int NUM_INTERVALS = 4;

/**
 * Values of the columns that will be discretized.
 */
float g_testColumns[NUM_COLUMNS][COLUMN_SIZE] = { {0.5, 2.2, 4.5, 1.2}, {-7.5, 0.0, 3.5, 1.2} };

/**
 * The correct discretization for the columns above.
 */
unsigned char g_targetColumns[NUM_COLUMNS][COLUMN_SIZE] = { {0, 1, 3, 0}, {0, 2, 3, 3} };

/**
 * Tests the functionality related to the discretizer.
 */
class ColumnDiscretizerTestSuite : public CxxTest::TestSuite
{
public:
	/**
	 * Simple test for the discretizer.
	 */
	void testDiscretizer()
	{
		ColumnData<float> riskFactors;
		addColumns( &riskFactors );

		ColumnData<unsigned char> discretizedRiskFactors;
		ColumnDiscretizer discretizer( NUM_INTERVALS );
		discretizer.transform( riskFactors, &discretizedRiskFactors );
		TS_ASSERT_EQUALS( discretizedRiskFactors.size( ), NUM_COLUMNS );

		assertDiscretization( discretizedRiskFactors );
	}

	/**
	 * Tests that if all elements are equal in a column, the
	 * discretized values should all lie in the last interval.
	 */
	void testSanity()
	{
		std::vector<float> singleColumn(1, 6.5);
		ColumnData<float> singleColumnData;
		singleColumnData.addColumn( singleColumn );

		ColumnData<unsigned char> discretizedColumnData;
		ColumnDiscretizer discretizer( NUM_INTERVALS );
		discretizer.transform( singleColumnData, &discretizedColumnData );

		TS_ASSERT_EQUALS( discretizedColumnData.size( ), 1 );
		TS_ASSERT_EQUALS( discretizedColumnData.numberOfRows( ), 1 );
		TS_ASSERT_EQUALS( discretizedColumnData.getColumn( 0 )[ 0 ], NUM_INTERVALS - 1 );
	}

	/**
	 * Add the columns that are going to be discretized to the
	 * given ColumnData.
	 *
	 * @param columnData The set of columns to be discretized is added
	 *                   to this object.
	 */
	void addColumns(ColumnData<float> *columnData)
	{
		for(unsigned int i = 0; i < NUM_COLUMNS; i++)
		{
			float *columnToAdd =  g_testColumns[i];
			std::vector<float> column( columnToAdd, columnToAdd + COLUMN_SIZE );
			columnData->addColumn( column );
		}
	}

	/**
	 * Help function that asserts that the discretization of a set
	 * of columns is correct.
	 *
	 * @param columns A set of columns that has been discretized.
	 */
	void assertDiscretization(ColumnData<unsigned char> &columns)
	{
		for(unsigned int i = 0; i < NUM_COLUMNS; i++)
		{
			const std::vector<unsigned char> &column = columns.getColumn( i );
			for(unsigned int j = 0; j < COLUMN_SIZE; j++)
			{
				TS_ASSERT_EQUALS( column[ j ], g_targetColumns[ i ][ j ] );
			}

		}
	}

};

#endif /* COLUMNDISCRETIZERTEST_H_ */
