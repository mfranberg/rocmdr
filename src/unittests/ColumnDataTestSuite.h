/*
 * ColumnDataTestSuite.h
 *
 *  Created on: Jul 6, 2011
 *      Author: fmattias
 */

#ifndef COLUMNDATATESTSUITE_H_
#define COLUMNDATATESTSUITE_H_

#include <vector>

#include <data/ColumnData.h>

#include <cxxtest/TestSuite.h>

class ColumnDataTestSuite : public CxxTest::TestSuite
{
public:

	/**
	 * Simple test for the constructor.
	 */
	void testColumnCreation(void)
	{
		ColumnData<int> columns;
		TS_ASSERT_EQUALS( columns.size( ), 0 );
	}

	/**
	 * Simple test for adding a column.
	 */
	void testColumnAdd(void)
	{
		std::vector<int> col1( 2, 1 );
		std::vector<int> col2( 2, 2 );
		std::vector<int> col3( 2, 3 );
		std::vector<int> badColumn( 3, -1 );

		ColumnData<int> columns;
		columns.addColumn( col1 );
		columns.addColumn( col2 );
		columns.addColumn( col3 );
		TS_ASSERT_EQUALS( columns.size( ), 3 );

		TS_ASSERT_THROWS( columns.addColumn( badColumn ), ColumnData<int>::IncompatibleColumnException );
	}

	/**
	 * Tests getting a single column.
	 */
	void testColumnGet(void)
	{
		std::vector<int> col1( 2, 1 );

		ColumnData<int> columns;
		columns.addColumn( col1 );

		const std::vector<int> column = columns.getColumn( 0 );
		TS_ASSERT_EQUALS( column.size( ), 2 );
		TS_ASSERT_EQUALS( column[0], 1 );
		TS_ASSERT_EQUALS( column[1], 1 );

	}

	/**
	 * Simple test for getting a subset of the columns.
	 */
	void testColumnGetSubset(void)
	{
		std::vector<int> col0(1, 0);
		std::vector<int> col1(1, 1);
		std::vector<int> col2(1, 2);
		std::vector<int> col3(1, 3);

		ColumnData<int> columns;
		columns.addColumn( col0 );
		columns.addColumn( col1 );
		columns.addColumn( col2 );
		columns.addColumn( col3 );

		ColumnData<int> subset;
		columns.getColumnSubset( 1, 2, &subset );
		TS_ASSERT_EQUALS( subset.size( ), 2 );

		const std::vector<int> tmpColumn1 = subset.getColumn( 0 );
		TS_ASSERT_EQUALS( tmpColumn1.at( 0 ), 1 );
		const std::vector<int> tmpColumn2 = subset.getColumn( 1 );
		TS_ASSERT_EQUALS( tmpColumn2.at( 0 ), 2 );
	}
};

#endif /* COLUMNDATATESTSUITE_H_ */
