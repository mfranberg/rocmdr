/*
 * AucAnalysisTest.h
 *
 *  Created on: Jul 7, 2011
 *      Author: fmattias
 */

#ifndef ROCMDRANALYSISTEST_H_
#define ROCMDRANALYSISTEST_H_

#include <data/ColumnData.h>
#include <data/PhenotypeMapping.h>
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
unsigned char g_riskFactors[NUM_COLUMNS][COLUMN_SIZE] = { {0, 0, 0, 1, 0, 0, 1, 1},
														 {0, 0, 0, 0, 1, 1, 1, 1} };
bool g_phenotypes[COLUMN_SIZE] = {1, 0, 0, 1, 0, 0, 1, 1};

static const float TARGET_AUC = 0.9375f;

class RocMdrAnalysisTest : public CxxTest::TestSuite
{
public:
	void testAucAnalysis()
	{
		ColumnData<unsigned char> riskFactors;
		setupRiskFactors( &riskFactors );

		std::vector<bool> phenotypes;
		setupPhenotypes( &phenotypes );

		RocMdrAnalysis rocMdrAnalysis( riskFactors, PhenotypeMapping( phenotypes ) );
		TS_ASSERT_DELTA( rocMdrAnalysis.getAuc( ), TARGET_AUC, 0.0001f );
	}

	void setupRiskFactors(ColumnData<unsigned char> *riskFactors)
	{
		for(unsigned int i = 0; i < NUM_COLUMNS; i++)
		{
			unsigned char *columnData = g_riskFactors[ i ];
			std::vector<unsigned char> column( columnData, columnData + COLUMN_SIZE );
			riskFactors->addColumn( column );
		}
	}

	void setupPhenotypes(std::vector<bool> *phenotypes)
	{
		phenotypes->assign( g_phenotypes, &g_phenotypes[0] + COLUMN_SIZE );
	}
};

#endif /* ROCMDRANALYSISTEST_H_ */
