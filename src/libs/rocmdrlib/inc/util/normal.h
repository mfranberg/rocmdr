/*
 * normal.h
 *
 *  Created on: Feb 3, 2012
 *      Author: fmattias
 */

#ifndef NORMAL_H_
#define NORMAL_H_

/**
 * Evaluates the cumulative distribution function for a
 * Normal(0, 1) variable at x, and it's value.
 *
 * @param x The location to evaluate the cdf.
 *
 * @return The cumulative probability mass up to x.
 */
double normcdf(double x);

#endif /* NORMAL_H_ */
