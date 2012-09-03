/*
 * SplitIndices.h
 *
 *  Created on: Aug 30, 2012
 *      Author: fmattias
 */

#ifndef SPLITINDICES_H_
#define SPLITINDICES_H_

#include <utility>

/**
 * Simple class that represents a range of iterations.
 */
typedef std::pair<size_t, size_t> Range;

/**
 * Determines which indices a partial iterator with the
 * given index should iterate over.
 *
 * @param start Start iteration.
 * @param stop Stop iteration.
 * @param index Index of this partial iterator.
 * @param numParts Number of partial iterators.
 */
Range splitIndices(size_t start, size_t stop, size_t index, size_t numParts);

#endif /* SPLITINDICES_H_ */
