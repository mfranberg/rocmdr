/*
 * AucAnalysisTest.h
 *
 *  Created on: Jul 7, 2011
 *      Author: fmattias
 */

#ifndef INTERACTIONBATCHTEST_H_
#define INTERACTIONBATCHTEST_H_

#include <Config.h>
#include <data/ColumnData.h>
#include <data/PhenotypeMapping.h>
#include <iter/AllIterator.h>
#include <InteractionBatch.h>
#include <RocMdrResult.h>

#include <algorithm>

#include <cxxtest/TestSuite.h>

/**
 * Number of snps.
 */
static const unsigned int NUM_SNPS = 11;

/**
 * Number of samples.
 */
static const unsigned int NUM_SAMPLES = 20;

class InteractionBatchTest : public CxxTest::TestSuite
{
public:
	InteractionBatchTest()
	{
		srandom( 5215 );
	}

	void testNoInteraction()
	{
		ColumnData<unsigned char> snps;
		randomSnps( &snps, NUM_SNPS, NUM_SAMPLES );

		std::vector<bool> phenotypes;
		randomPhenotypes( &phenotypes, NUM_SAMPLES );
		PhenotypeMapping mapping( phenotypes );

		InteractionBatch batch;
		AllIterator iter( snps, mapping, 1 );
		std::vector<RocMdrResult> results = batch.run( &iter, 1 );

		TS_ASSERT_EQUALS( results.size( ), snps.size( ) );
	}

	void testInteraction()
	{
		ColumnData<unsigned char> snps;
		randomSnps( &snps, NUM_SNPS, NUM_SAMPLES );

		std::vector<bool> phenotypes;
		randomPhenotypes( &phenotypes, NUM_SAMPLES );
		PhenotypeMapping mapping( phenotypes );

		InteractionBatch batch;
		AllIterator iter( snps, mapping, 2 );
		std::vector<RocMdrResult> results = batch.run( &iter, 1 );

		TS_ASSERT_EQUALS( results.size( ), snps.size( ) * ( snps.size( ) - 1) / 2 );
		for(unsigned int i = 0; i < results.size( ); i++)
		{
			TS_ASSERT_LESS_THAN( results[ i ].getAuc( ), 1.0f );
			TS_ASSERT_LESS_THAN( 0.49, results[ i ].getAuc( ) );
		}
	}

	void testThreads()
	{
		ColumnData<unsigned char> snps;
		randomSnps( &snps, NUM_SNPS, NUM_SAMPLES );

		std::vector<bool> phenotypes;
		randomPhenotypes( &phenotypes, NUM_SAMPLES );
		PhenotypeMapping mapping( phenotypes );

		InteractionBatch batch;
		AllIterator iter( snps, mapping, 2 );
		std::vector<RocMdrResult> results = batch.run( &iter, 1 );
		std::vector<RocMdrResult> resultsThread = batch.run( &iter, 2 );

		TS_ASSERT_EQUALS( results.size( ), resultsThread.size( ) );

		std::sort( results.begin( ), results.end( ) );
		std::sort( resultsThread.begin( ), resultsThread.end( ) );

		for(unsigned int i = 0; i < results.size( ); i++)
		{
			TS_ASSERT_DELTA( results[ i ].getAuc( ), resultsThread[ i ].getAuc( ), 0.00001f );
		}
	}

	void randomSnps(ColumnData<unsigned char> *snps, unsigned int numSnps, unsigned int numSamples)
	{
		for(unsigned int i = 0; i < numSnps; i++)
		{
			std::vector<unsigned char> column;
			for(unsigned int j = 0; j < numSamples; j++)
			{
				column.push_back( random( ) % 3 );
			}

			snps->addColumn( column );
		}
	}

	void randomPhenotypes(std::vector<bool> *phenotypes, unsigned int numSamples)
	{
		phenotypes->resize( numSamples, false );
		for(unsigned int i = 0; i < numSamples; i++)
		{
			(*phenotypes)[ i ] = random( ) % 2;
		}
	}
};

#endif /* INTERACTIONBATCHTEST_H_ */
