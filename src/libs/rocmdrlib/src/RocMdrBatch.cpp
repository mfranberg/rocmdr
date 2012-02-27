/*
 * RocMdrBatch.cpp
 *
 *  Created on: Feb 16, 2012
 *      Author: fmattias
 */
#include <RocMdrAnalysis.h>

#include <RocMdrBatch.h>

void
RocMdrBatch::runRocMdrRecursive(unsigned int maxLevel,
					   ColumnData<unsigned char> &allSnps,
					   PhenotypeMapping &phenotypes,
					   ColumnData<unsigned char> &curSnps,
					   std::vector<unsigned int> &curLoci,
					   std::vector<RocMdrResult> *results)
{
	unsigned int start = 0;
	if( curLoci.size( ) > 0 )
	{
		start = curLoci.back( ) + 1;
	}

	if( curSnps.size( ) == maxLevel - 1 )
	{
		for(unsigned int i = start; i < allSnps.size( ); i++)
		{
			curSnps.addColumn( allSnps.getColumn( i ) );
			curLoci.push_back( i );

			RocMdrAnalysis rocMdr( curSnps, phenotypes );
			float auc = rocMdr.getAuc( );
			RocMdrResult result( curLoci, auc, m_nullSimulator.computePValue( auc ) );

			results->push_back( result );

			curLoci.pop_back( );
			curSnps.removeColumnLast( );
		}
	}
	else
	{
		for(unsigned int i = start; i < allSnps.size( ); i++)
		{
			curSnps.addColumn( allSnps.getColumn( i ) );
			curLoci.push_back( i );

			runRocMdrRecursive( maxLevel, allSnps, phenotypes, curSnps, curLoci, results );

			curLoci.pop_back( );
			curSnps.removeColumnLast( );
		}
	}
}

std::vector<RocMdrResult>
RocMdrBatch::run(unsigned int interactionOrder,
					  ColumnData<unsigned char> snps,
					  PhenotypeMapping phenotypes)
{
	std::vector<RocMdrResult> results;
	ColumnData<unsigned char> empty;
	std::vector<unsigned int> loci;

	m_nullSimulator.setData( snps, phenotypes.getPhenotypes( ) );
	m_nullSimulator.setOrder( interactionOrder );

	runRocMdrRecursive( interactionOrder, snps, phenotypes, empty, loci, &results );

	return results;
}
