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

class CSVParser {
  public:
    static std::vector<std::vector<std::string>> readCSV(const std::string& filename);
    static std::vector<std::string> getRowById(const std::string& filename, int id);
    static std::vector<std::string> getRowByAbbreviation(const std::string& filename, const std::string& abbreviation);
};


#endif //CSVPARSER_HPP
