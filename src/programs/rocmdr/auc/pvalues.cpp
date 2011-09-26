/*
 * pvalues.cpp
 *
 *  Created on: Sep 26, 2011
 *      Author: fmattias
 */
#include <iostream>
#include <fstream>

#include "PValueSimulator.h"
#include "pvalues.h"

/**
 * The file format for simulated values is:
 * <Number of columns> <Number of simulations>
 * <List of column ranges separated by "\n">
 * <List of simulations separated by "\n">
 */

/**
 * Reads a list of simulated AUCs from the given file.
 * This function fails if the file does not have the same set of columnRanges, or
 * if it is empty.
 *
 * @param filename The file containing the simulations.
 * @param columnRanges The number of distinct values in each column.
 * @param simulatedAUC The simulations will be added here.
 *
 * @return true if the file could be read and the columnRanges matched,
 *         false otherwise.
 */
static
bool
readSimulationFromFile(const char *filename, const std::vector<unsigned int> &columnRanges, std::vector<float> *simulatedAUC)
{
	std::ifstream file( filename );
	if( !file.good( ) )
	{
		return false;
	}

	unsigned int numColumns;
	unsigned int numSimulations;
	file >> numColumns >> numSimulations;

	// Read column ranges
	std::vector<unsigned int> columnRangesInFile;
	for(unsigned int i = 0; i < numColumns; i++)
	{
		unsigned int range;
		file >> range;
		columnRangesInFile.push_back( range );
	}

	if( columnRanges != columnRangesInFile )
	{
		file.close( );
		return false;
	}

	// Read the simulated AUCs
	for(unsigned int i = 0; i < numSimulations; i++)
	{
		float auc;
		file >> auc;
		simulatedAUC->push_back( auc );
	}

	file.close( );
	return true;
}

/**
 * Writes a set of simulated AUCs to a file.
 *
 * @param filename The file to write to.
 * @param columnRanges The number of distinct values in each column.
 * @param simluatedAUC The simulations that should be written to file.
 */
static
void
writeSimulationToFile(const char *filename, const std::vector<unsigned int> &columnRanges, const std::vector<float> &simulatedAUC)
{
	std::ofstream file( filename );

	file << columnRanges.size( ) << " " << simulatedAUC.size( ) << std::endl;

	for(unsigned int i = 0; i < columnRanges.size( ); i++)
	{
		file << columnRanges[ i ] << std::endl;
	}

	for(unsigned int i = 0; i < simulatedAUC.size( ); i++)
	{
		file << simulatedAUC[ i ] << std::endl;
	}

	file.close( );
}

/**
 * Internal helper function that computes a p-value for each AUC
 * value in the given AUC list.
 *
 * @param aucValues The AUC values to compute p-values for.
 * @param simulator The simulator used to provide the p-values.
 *
 * @return A list of p-values for each AUC value.
 */
static
std::vector<float>
simluatePValues(std::vector<float> aucValues, PValueSimulator *simulator)
{
	std::vector<float> pValues;

	for(unsigned int i = 0; i < aucValues.size( ); i++)
	{
		pValues.push_back( simulator->computePValue( aucValues[ i ] ) );
	}

	return pValues;
}

std::vector<float>
simulatePValues(const std::vector<float> &aucValues,
				const std::vector<unsigned int> &columnRanges,
				unsigned int numSamples,
				const char *filename)
{
	PValueSimulator simulator( columnRanges, numSamples );

	std::vector<float> preSimulatedAUC;
	if( readSimulationFromFile( filename, columnRanges, &preSimulatedAUC ) )
	{
		simulator.setSimulation( preSimulatedAUC );
	}
	else
	{
		writeSimulationToFile( filename, columnRanges, simulator.getSimulation( ) );
	}

	return simluatePValues( aucValues, &simulator );
}
