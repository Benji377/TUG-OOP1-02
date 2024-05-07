//---------------------------------------------------------------------------------------------------------------------
//
// The CSV Parser reads a CSV file and stores the data in a vector of vectors. It can read the entire file at once or
// read it line by line. The data can be accessed by row and column. It furthermore can search for a specific value in
// the data. For example, it can return the row where the column has a specific value. All functions are static.
//
// Group: 068
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef CSVPARSER_HPP
#define CSVPARSER_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

class CSVParser {
  public:
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Reads a CSV file and stores the data in a vector of vectors. The first vector contains the rows and the second
    /// vector contains the columns. The data can be accessed by row and column.
    /// @param filename The name of the CSV file
    /// @return A vector of vectors containing the data of the CSV file
    //
    static std::vector<std::vector<std::string>> readCSV(const std::string& filename);

    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Reads a CSV file and returns the row where the first column has a specific value. The abbreviation is normally
    /// used to identify a row. The function calls readCSV() to get the data.
    /// @param filename The name of the CSV file
    /// @param abbreviation The value to search for in the first column
    /// @return A vector containing the row where the abbreviation was found
    //
    static std::vector<std::string> getRowByAbbreviation(const std::string& filename, const std::string& abbreviation);

    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Reads all CSV files one by one and looks for the abbreviation in the first column. If the abbreviation is found,
    /// the function returns the item. The function calls getRowByAbbreviation() to get the row.
    /// @param abbreviation The value to search for in the first column
    /// @return The item that was found
    //
    static std::string getFolderByAbbreviation(const std::string& abbreviation);
};


#endif //CSVPARSER_HPP
