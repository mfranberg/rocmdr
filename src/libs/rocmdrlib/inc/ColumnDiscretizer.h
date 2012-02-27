/*
 * ColumnDiscretizer.h
 *
 *  Created on: Jul 6, 2011
 *      Author: fmattias
 */

#include <ColumnTransformer.h>

/**
 * A type of transformer that discretizes column data such
 * that each column has the same number of intervals. The
 * discretized data will always be positive regardless of
 * the input data.
 */
class ColumnDiscretizer
: public ColumnTransformer<float, unsigned char>
{
public:
	/**
	 * Constructor.
	 *
	 * @param numIntervals The number of intervals each column will be
	 *                     discretized equally into.
	 */
	ColumnDiscretizer(unsigned int numIntervals);

	/**
	 * Performs the configured discretization.
	 *
	 * @param before The continuous columns.
	 * @param after The discretized columns, the discretized columns will
	 *              be added last.
	 */
	void transform(ColumnData<float> &before, ColumnData<unsigned char> *after);

private:
	/**
	 * Discretizes the given column such that each column will have the
	 * same number of intervals.
	 *
	 * If all values are equal, they will all lie in the last interval.
	 *
	 * @param column The column to discretize.
	 * @param numIntervals The number of intervals to discretize each column in.
	 * @param discretizedColumn The discretized values will end up here.
	 */
	void discretizeColumn(const std::vector<float> &column,
						unsigned int numIntervals,
						std::vector<unsigned char> *discretizedColumn);

	/**
	 * The number of intervals to discretize each column into.
	 */
	unsigned int m_numIntervals;

};
