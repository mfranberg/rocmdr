/*
 * PhenotypeMappingTest.h
 *
 *  Created on: Jul 8, 2011
 *      Author: fmattias
 */

#ifndef PHENOTYPEMAPPINGTEST_H_
#define PHENOTYPEMAPPINGTEST_H_

#include <vector>

#include <PhenotypeMapping.h>

#include <cxxtest/TestSuite.h>

/**
 * Simple tests for the PhenotypeMapping class.
 */
class PhenotypeMappingTest : public CxxTest::TestSuite
{
public:
	/**
	 * Tests the getNumPositive and getNumNegative
	 * functions.
	 */
	void testGetNumPositiveAndNegative()
	{
		std::vector<bool> phenotypes;
		phenotypes.push_back( true );
		phenotypes.push_back( false );
		phenotypes.push_back( true );

		PhenotypeMapping mapping( phenotypes );

		TS_ASSERT_EQUALS( mapping.getNumPositive( ), 2u );
		TS_ASSERT_EQUALS( mapping.getNumNegative( ), 1u );
	}

	/**
	 * Tests the isPositive function.
	 */
	void testIsPositive()
	{
		std::vector<bool> phenotypes;
		phenotypes.push_back( true );
		phenotypes.push_back( false );

		PhenotypeMapping mapping( phenotypes );

		TS_ASSERT_EQUALS( mapping.isPositive( 0 ), true );
		TS_ASSERT_EQUALS( mapping.isPositive( 1 ), false );
	}
};

#endif /* PHENOTYPEMAPPINGTEST_H_ */
