/*
 * CSVIntProcessorTest.h
 *
 *  Created on: Sep 20, 2011
 *      Author: fmattias
 */

#ifndef CSVINTPROCESSORTEST_H_
#define CSVINTPROCESSORTEST_H_

#include <CSVIntProcessor.h>

#include <cxxtest/TestSuite.h>

/**
 * A couple of simple tests for the class
 * that converts CSV data to integer columns.
 */
class CSVIntProcessorTest : public CxxTest::TestSuite
{
public:

	void setUp()
	{
		m_processor = new CSVIntProcessor( );
	}

	void tearDown()
	{
		delete m_processor;
		m_processor = NULL;
	}

	/**
	 * Tests that an exception is thrown on a bad field.
	 */
	void testBadField()
	{
		m_processor->onRowEnd( );
		TS_ASSERT_THROWS_ANYTHING( m_processor->onField( "3.14" ) );
	}

	/**
	 * Tests that a good field is converted correctly.
	 */
	void testGoodField()
	{
		TS_ASSERT_THROWS_NOTHING( m_processor->onField( "44" ) );
		m_processor->onRowEnd( );

		TS_ASSERT_EQUALS( m_processor->getColumnData( ).size( ), 1 );
		TS_ASSERT_EQUALS( m_processor->getColumnData( )[ 0 ][ 0 ], 44 );
	}

	/**
	 * Tests that a column with a header is correct.
	 */
	void testGoodFieldWithHeader()
	{
		TS_ASSERT_THROWS_NOTHING( m_processor->onField( "Column" ) );
		m_processor->onRowEnd( );

		TS_ASSERT_THROWS_NOTHING( m_processor->onField( "44" ) );
		m_processor->onRowEnd( );

		TS_ASSERT_EQUALS( m_processor->getColumnData( ).size( ), 1 );
		TS_ASSERT_EQUALS( m_processor->getHeader( ).size( ), 1 );

		TS_ASSERT_EQUALS( m_processor->getColumnData( )[ 0 ][ 0 ], 44 );
		TS_ASSERT( m_processor->getHeader( )[ 0 ] == "Column" );
	}

private:
	CSVIntProcessor *m_processor;
};

#endif /* CSVINTPROCESSORTEST_H_ */
