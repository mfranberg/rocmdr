/*
 * Config.h
 *
 *  Created on: Apr 25, 2012
 *      Author: fmattias
 */

#ifndef CONFIG_H_
#define CONFIG_H_

class Config
{
public:
	/**
	 * Sets the number of threads to be used.
	 *
	 * @param numThreads The number of threads to use.
	 */
	void setNumThreads(unsigned int numThreads);

	/**
	 * Returns the number of threads.
	 *
	 * @return the number of threads.
	 */
	unsigned int getNumThreads();

	/**
	 * Sets the number of simulations.
	 *
	 * @param numSimulations The number of simulations.
	 */
	void setNumSimulations(unsigned int numSimulations);

	/**
	 * Returns the number of simulations.
	 *
	 * @return the number of simulations.
	 */
	unsigned int getNumSimulations();

	/**
	 * Returns the global config singleton.
	 *
	 * @return the global config singleton.
	 */
	static Config *getConfig();

private:
	/**
	 * Private constructor.
	 */
	Config();

	/**
	 * Reference to the singleton.
	 */
	static Config *m_singleton;

	/**
	 * Number of threads to use.
	 */
	unsigned int m_numThreads;

	/**
	 * The number of simulations.
	 */
	unsigned int m_numSimulations;
};

#endif /* CONFIG_H_ */
