##
# A simple tool for extracting columns from a text file with
# the following structure:
# * First line contains a list of column names separated by whitespace.
# * Rows are separated by new lines.
# * Column values are separated by whitespace.
#
import argparse
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
        config_parser.readfp(config_file)
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
        try:
            index = lowered_column_list.index(name)
            indices.append(index)
        except ValueError:
            print("No column '{0}' in input file.".format(name))
    
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

##
# Prints the extracted columns to stdout, beginning with
# a line containing just the column names.
#
# @param column_names Names of the columns.
# @param extracted A list of rows, where each row is a list of
#                  the column values.
#
def print_columns(column_names, extracted_columns):
    print("\t".join(column_names))
    for row in extracted_columns:
        print("\t".join(row))

##
# Parses the command line arguments and returns them.
#
def parse_arguments():
    argument_parser = argparse.ArgumentParser(description="Extract a subset of the columns in a tab delimited data file.")
    argument_parser.add_argument("column_config", metavar="column_config", type=argparse.FileType('r'), nargs=1, help="Configuration file containing the columns that should be extracted.")
    argument_parser.add_argument("column_file", metavar="column_file", type=argparse.FileType('r'), nargs=1, help="The file that contains the data.")
    return argument_parser.parse_args()

##
# Main entrypoint
#
def main(argv):
    parsed_args = parse_arguments()

    column_config = ColumnConfigFactory().create_from_config(parsed_args.column_config[0])
    extracted_columns = extract_columns(column_config, parsed_args.column_file[0])

    print_columns(column_config.get_columns_to_extract(), extracted_columns)

##
# Only start script if started from a shell.
#
if __name__ == "__main__":
    if sys.version_info[0] < 2 or (sys.version_info[0]== 2 and sys.version_info[1] <= 6) :
        print("You need at least python version 2.7 to run this script")
        exit()


    main(sys.argv)

