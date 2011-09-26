/*
 * parsing.h
 *
 *  Created on: Sep 26, 2011
 *      Author: fmattias
 */

#ifndef PARSING_H_
#define PARSING_H_

#include <vector>

#include <ColumnData.h>

/**
 * Reads and populates a ColumnData object from a file. It assumes
 * that each column only contains unsigned integers. It also assumes
 * that the format of the file is CSV, where the separator is a tab.
 *
 * @param path The path to the file to read from.
 * @param data The read data will be stored here.
 */
void readToColumnData(const char *path, ColumnData<unsigned int> *data);

/**
 * Reads phenotypes from a file to a vector. It assumes that it is
 * case/control data. It also assumes that the format of the file is
 * CSV, where the separator is a tab. If there are multiple columns
 * only the first one will be used.
 *
 * @param path The path to the file to read from.
 * @param phenotypes The case/control data will be stored here.
 */
void readPhenotypes(const char *path, std::vector<bool> *phenotypes);

#endif /* PARSING_H_ */
