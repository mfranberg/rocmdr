/*
 * RocMdrVisitor.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: fmattias
 */

#include <Config.h>
#include <RocMdrAnalysis.h>
#include <RocMdrVisitor.h>

RocMdrVisitor::RocMdrVisitor()
: m_nullSimulator( Config::getConfig( )->getNumSimulations( ) )
{

}

void
RocMdrVisitor::visit(ColumnData<unsigned char> &snps, std::vector<unsigned int> &indices, PhenotypeMapping &phenotypes)
{
	RocMdrAnalysis rocMdr( snps, phenotypes );

	float auc = rocMdr.getAuc( );
	float pValue = m_nullSimulator.getPValue( auc, snps, phenotypes );
	RocMdrResult result( indices, auc, pValue );

	m_results.push_back( result );
}

std::vector<RocMdrResult>
RocMdrVisitor::getResults()
{
	return m_results;
}
