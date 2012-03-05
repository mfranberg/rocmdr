/*
 * ColumnTransformer.h
 *
 *  Created on: Jul 6, 2011
 *      Author: fmattias
 */

#ifndef COLUMNTRANSFORMER_H_
#define COLUMNTRANSFORMER_H_

#include <data/ColumnData.h>

/**
 * Interface for transforming data in columns, between
 * different types or different formats.
 */
template <class T_from, class T_to = T_from>
class ColumnTransformer
{
public:
	/**
	 * Transforms a set of columns to another set of columns.
	 *
	 * @param before The input columns.
	 * @param after The ouput columns.
	 */
	virtual void transform(ColumnData<T_from> &before, ColumnData<T_to> *after) = 0;
};


#endif /* COLUMNTRANSFORMER_H_ */
