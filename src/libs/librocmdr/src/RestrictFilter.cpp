/*
 * RestrictFilter.cpp
 *
 *  Created on: Apr 9, 2012
 *      Author: fmattias
 */

#include <RestrictFilter.h>

RestrictFilter::RestrictFilter(const std::vector<unsigned int> &skipList)
: m_skipSet( skipList.begin( ), skipList.end( ) )
{

}


bool
RestrictFilter::skip(unsigned int index)
{
	return m_skipSet.count( index ) > 0;
}
