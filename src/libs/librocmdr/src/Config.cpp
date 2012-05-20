/*
 * Config.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: fmattias
 */

#include <Config.h>

Config *Config::m_singleton = 0;

Config::Config()
: m_numThreads( 1 ),
  m_numSimulations( 100 )
{

}

void
Config::setNumThreads(unsigned int numThreads)
{
	m_numThreads = numThreads;
}

unsigned int
Config::getNumThreads()
{
	return m_numThreads;
}

void
Config::setNumSimulations(unsigned int numSimulations)
{
	m_numSimulations = numSimulations;
}

unsigned int
Config::getNumSimulations()
{
	return m_numSimulations;
}

Config *
Config::getConfig()
{
	if( m_singleton == 0 )
	{
		m_singleton = new Config( );
	}

	return m_singleton;
}

