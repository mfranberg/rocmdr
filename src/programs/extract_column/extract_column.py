##
# A simple tool for extracting columns from a text file with
# the following structure:
# * First line contains a list of column names separated by whitespace.
# * Rows are separated by new lines.
# * Column values are separated by whitespace.
#
import csv
import sys
import os.path

import configparser

##
# A factory that creates ColumnConfigs from files.
#
class ColumnConfigFactory:
    COLUMN_LIST = "columnlist"

    ##
    # Creates a column config from the given config file.
    #
    # @param config_file The configuration file to read.
    # @return The config file that was created, an empty one will
    #          be created on error.
    #
    def create_from_config(self, config_file):
        config_parser = configparser.RawConfigParser(allow_no_value=True)
        parsed_files = config_parser.read([config_file])
        if len(parsed_files) != 1:
            print("Warning: Config could not be read.")
            return ColumnConfig([])

        columns_to_extract = config_parser.options(self.COLUMN_LIST)

        return ColumnConfig(columns_to_extract)

##
# Holds the configuration for the current extraction.
#
class ColumnConfig:
   
    ##
    # Constructor
    #
    # @param columns_to_extract List of the names of the columns to extract.
    #
    def __init__(self, columns_to_extract):
        self.columns_to_extract = columns_to_extract
        self.should_replace_missing = True

    ##
    # Changes whether the missing values should be converted to R format.
    #
    # @param should_replace Determines whether missing values (-9) should
    #                       be replaced with the R format ("NA").
    #
    #
    def set_should_replace_missing(self, should_replace):
        self.should_replace_missing = should_replace

    ##
    # Optionally changes a column value and returns it. Can be used
    # to make sure column values are sane.
    #
    # @param column_value The column value that might be reformatted.
    # @return The new column value.
    #
    def reformat_column_value(self, column_value):
        if self.should_replace_missing and column_value == "-9":
            return "NA"
        else:
            return column_value

    ##
    # Returns the names of the columns to extract.
    #
    # @return the names of the columns to extract.
    #
    def get_columns_to_extract(self):
        return self.columns_to_extract

##
# Returns the a list of indices of columns with the given names.
#
# @param columns_to_extract Name of the columns to be searched for.
# @param column_list List of names of all the columns.
# @return The list of indices of the columns that are going to be extracted.
#
def find_column_indices(columns_to_extract, column_list):
    lowered_columns_to_extract = list(map(str.lower, columns_to_extract))
    lowered_column_list = list(map(str.lower, column_list))

    indices = []
    for name in lowered_columns_to_extract:
        index = lowered_column_list.index(name)
        if index != -1:
            indices.append(index)
    
    return indices

##
# Returns the values in the column with the given name, from an
# opened file.
#
# @param column_name Name of the column to retrieve values from.
# @param column_file The file object that contains the column data.
#
def extract_columns(column_config, column_file):
    csv_reader = csv.reader(column_file, delimiter = '\t')
    column_list = next(csv_reader)
    column_indices = find_column_indices(column_config.get_columns_to_extract(), column_list)

    extracted_columns = []
    for row in csv_reader:
        restricted_row = [column_config.reformat_column_value(row[i]) for i in column_indices]
        extracted_columns.append(restricted_row)

    return extracted_columns


def print_columns(column_names, extracted_columns):
    print("\t".join(column_names))
    for row in extracted_columns:
        print("\t".join(row))

##
# Main entrypoint
#
def main(argv):
    if len(argv) != 3:
        print("Usage: extract_column.py column_config column_file")
        return
    
    if not os.path.exists(argv[1]):
        print("No such file: ", argv[1])
        return
    if not os.path.exists(argv[2]):
        print("No such file: ", argv[2])
        return

    column_config_factory = ColumnConfigFactory()
    column_config = column_config_factory.create_from_config(argv[1])
    column_file = open(argv[2], 'r')
    
    extracted_columns = extract_columns(column_config, column_file)
    column_file.close()

    print_columns(column_config.get_columns_to_extract(), extracted_columns)

##
# Only start script if started from a shell.
#
if __name__ == "__main__":
    if sys.version_info[0] < 2 or (sys.version_info[0]== 2 and sys.version_info[1] <= 6) :
        print("You need at least python version 2.7 to run this script")
        exit()


    main(sys.argv)

