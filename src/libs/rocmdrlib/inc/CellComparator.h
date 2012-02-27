/*
 * CellComparator.h
 *
 *  Created on: Feb 3, 2012
 *      Author: fmattias
 */

#ifndef CELLCOMPARATOR_H_
#define CELLCOMPARATOR_H_

#include <Cell.h>

/**
 * This class is used to sort the cells. The cells
 * are sorted according to the their slope, but there
 * are some cases when the slope is infinite, which is
 * handled by just comparing the number of true positives.
 */
class CellComparator
{
public:
	/**
	 * Constructor.
	 *
	 * @param numPositives Number of positive samples.
	 * @param numNegatives Number of negative samples.
	 */
	CellComparator(unsigned int numPositives, unsigned int numNegatives)
	: m_numPositives( numPositives ),
	  m_numNegatives( numNegatives )
	{

	}

	bool operator()(const Cell &a, const Cell& b) const
	{
		if( a.numFp != 0 && b.numFp != 0 )
		{
			return computeSlope( a ) < computeSlope( b );
		}
		else if( a.numFp == 0 && b.numFp != 0 )
		{
			// Cell with 0 fp < Cell with more than 0 fp
			return false;
		}
		else if( a.numFp != 0 && b.numFp == 0 )
		{
			// Cell with more than 0 fp < Cell with 0 fp
			return true;
		}
		else
		{
			// Cell with 0 fp < Cell with 0 fp
			return a.numTp < b.numTp;
		}
	}

private:
	/**
	 * Computes the tpr / fpr slope at a given point.
	 */
	float computeSlope(const Cell &cell) const
	{
		return (cell.numTp / m_numPositives) / (cell.numFp / m_numNegatives);
	}

	/**
	 * Number of positive samples.
	 */
	float m_numPositives;

	/**
	 * Number of negative samples.
	 */
	float m_numNegatives;
};

#endif /* CELLCOMPARATOR_H_ */
