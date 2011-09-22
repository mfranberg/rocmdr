/*
 * ColumnData.h
 *
 *  Created on: Jul 6, 2011
 *      Author: fmattias
 */

#ifndef COLUMNDATA_H_
#define COLUMNDATA_H_

#include <exception>
#include <stdexcept>
#include <vector>

/**
 * A representation of a set of columns containing
 * any kind of data. It seemed appropriate to structure
 * the data in columns, since will need to compute things
 * on subsets of the columns.
 *
 * Note: there are no const methods in order to allow
 * a subclass to initialize the columns lazily.
 */
template <class T>
class ColumnData
{
public:
	class IncompatibleColumnException
	: public std::exception
	{
		virtual const char* what() const throw()
		{
			return "You added a column that has a different length than the others.";
		}
	};

	/**
	 * Constructor.
	 *
	 * @param maxSize The maximum number of columns.
	 */
	ColumnData(int maxSize = 0);

	/**
	 * Adds the given column last.
	 *
	 * @param column The column to add.
	 */
	void addColumn(const std::vector<T> column);

	/**
	 * Adds the given column so that it gets index 0.
	 *
	 * @param column The column to add first.
	 */
	void addColumnFirst(const std::vector<T> column);

	/**
	 * Removes the last column.
	 */
	void removeColumnLast();

	/**
	 * Returns the column specified by the given index.
	 *
	 * @param columnIndex The 0-based index of the column.
	 * @return The vector representing the column.
	 */
	const std::vector<T> & getColumn(unsigned int columnIndex);

	/**
	 * Retrieves a subset of the columns and forms a new
	 * ColumnData object for them.
	 *
	 * @param start The first index of the range of columns to get.
	 * @param last The last index of the range of columns to get.
	 * @param columnSubset The ColumnData object where the subset will be stored.
	 */
	void getColumnSubset(unsigned int start, unsigned int end, ColumnData<T> *columnSubset);

	/**
	 * Returns the number of columns.
	 *
	 * @return The number of columns.
	 */
	unsigned int size();

	/**
	 * Returns the number of rows.
	 *
	 * @return The number of rows, will return 0 if there are no
	 *         columns.
	 */
	unsigned int numberOfRows();

private:
	/**
	 * List of all the columns.
	 */
	std::vector< std::vector<T> > m_columns;
};

template <class T>
ColumnData<T>::ColumnData(int maxSize)
{

}

template <class T>
void
ColumnData<T>::addColumn(const std::vector<T> column)
{
	if( numberOfRows( ) > 0 && numberOfRows( ) != column.size( ) )
	{
		throw IncompatibleColumnException( );
	}

	std::vector<T> columnCopy( column );
	m_columns.push_back( columnCopy );
}

template <class T>
void
ColumnData<T>::addColumnFirst(const std::vector<T> column)
{
	if( size( ) > 0 && numberOfRows( ) != column.size( ) )
	{
		throw IncompatibleColumnException( );
	}

	std::vector<T> columnCopy( column );
	m_columns.insert( m_columns.begin( ), columnCopy );
}

template <class T>
void
ColumnData<T>::removeColumnLast()
{
	m_columns.pop_back( );
}

template <class T>
const std::vector<T> &
ColumnData<T>::getColumn(unsigned int columnIndex)
{
	if( columnIndex >= size( ) )
	{
		throw std::range_error( "getColumn: Column index too high." );
	}

	return m_columns[ columnIndex ];
}

template <class T>
void
ColumnData<T>::getColumnSubset(unsigned int start, unsigned int end, ColumnData<T> *columnSubset)
{
	if( start >= size( ) || end >= size( ) || start > end )
	{
		throw std::range_error( "getColumnSubset: Range outside column range." );
	}

	for(unsigned int i = start; i <= end; i++)
	{
		columnSubset->addColumn( m_columns[i] );
	}

}

template <class T>
unsigned int
ColumnData<T>::size()
{
	return m_columns.size( );
}

template <class T>
unsigned int
ColumnData<T>::numberOfRows()
{
	if( m_columns.size( ) <= 0 )
	{
		return 0;
	}
	else
	{
		return m_columns[0].size( );
	}
}


#endif /* COLUMNDATA_H_ */
