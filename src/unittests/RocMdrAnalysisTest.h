/*
 * AucAnalysisTest.h
 *
 *  Created on: Jul 7, 2011
 *      Author: fmattias
 */

#ifndef ROCMDRANALYSISTEST_H_
#define ROCMDRANALYSISTEST_H_

#include <ColumnData.h>
#include <PhenotypeMapping.h>
#include <RocMdrAnalysis.h>

#include <cxxtest/TestSuite.h>

/**
 * Number of rows.
 */
static const unsigned int COLUMN_SIZE = 8;

/**
 * Number of columns.
 */
static const unsigned int NUM_COLUMNS = 2;

/**
 * The risk factors:
 *
 * ----------------
 * |  o   |    x  |
 * | o    |  x   Ê|
 * |      |       |
 * ----------------
 * |      |       |
 * |  x   |   x  Ê|
 * | o o  |       |
 * ----------------
 *
 * Which means that the roc curve will look like this:
 *
 *
 * |   *  *
 * *
 * *
 * |
 * -------->
 *
 * Giving an AUC of: 0.9375
 *
 */
unsigned int g_riskFactors[NUM_COLUMNS][COLUMN_SIZE] = { {0, 0, 0, 1, 0, 0, 1, 1},
														 {0, 0, 0, 0, 1, 1, 1, 1} };
bool g_phenotypes[COLUMN_SIZE] = {1, 0, 0, 1, 0, 0, 1, 1};

static const float TARGET_AUC = 0.9375f;

class RocMdrAnalysisTest : public CxxTest::TestSuite
{
public:
	void testAucAnalysis()
	{
		ColumnData<unsigned int> riskFactors;
		setupRiskFactors( &riskFactors );

		std::vector<bool> phenotypes;
		setupPhenotypes( &phenotypes );

		RocMdrAnalysis rocMdrAnalysis( riskFactors, PhenotypeMapping( phenotypes ) );
		TS_ASSERT_DELTA( rocMdrAnalysis.calculateAuc( ), TARGET_AUC, 0.0001f );

		std::vector< std::vector<unsigned int> > combinations;
		rocMdrAnalysis.getCombinations( &combinations );
		TS_ASSERT_EQUALS( combinations.size( ), 4u );
		TS_ASSERT( combinations[ 0 ][ 0 ] == 1u && combinations[ 0 ][ 1 ] == 1u );
	}

	void setupRiskFactors(ColumnData<unsigned int> *riskFactors)
	{
		for(unsigned int i = 0; i < NUM_COLUMNS; i++)
		{
			unsigned int *columnData = g_riskFactors[ i ];
			std::vector<unsigned int> column( columnData, columnData + COLUMN_SIZE );
			riskFactors->addColumn( column );
		}
	}

	void setupPhenotypes(std::vector<bool> *phenotypes)
	{
		phenotypes->assign( g_phenotypes, &g_phenotypes[0] + COLUMN_SIZE );
	}
};

#endif /* ROCMDRANALYSISTEST_H_ */
