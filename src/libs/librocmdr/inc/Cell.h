#ifndef CELL_H_
#define CELL_H_

/**
 * Small utility class for counting the number of
 * true and false positives in a cell.
 */
struct Cell
{
	/**
	 * Constructor.
	 */
	Cell()
	: numTp( 0 ),
	  numFp( 0 ),
	  lastIndividual( 0 )
	{

	}

	/**
	 * Number of true positives.
	 */
	unsigned int numTp;

	/**
	 * Number of false positives.
	 */
	unsigned int numFp;

	/**
	 * The index of the last individual who touched this.
	 */
	unsigned int lastIndividual;
};

#endif /* CELL_H_ */
