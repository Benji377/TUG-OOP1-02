#include "CSVParser.hpp"

std::vector<std::vector<std::string>> CSVParser::readCSV(const std::string& filename) {
  std::vector<std::vector<std::string>> data;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return data;
  }
  std::string line;
  while (std::getline(file, line)) {
    std::vector<std::string> row;
    std::stringstream ss(line);
    std::string cell;
    while (std::getline(ss, cell, ';')) {
      row.push_back(cell);
    }
    data.push_back(row);
  }
  file.close();
  return data;
}

std::vector<std::string> CSVParser::getRowByAbbreviation(const std::string& filename, const std::string& abbreviation) {
  std::vector<std::vector<std::string>> data = readCSV(filename);
  if (data.empty()) {
    return {};
  }
  // The abbreviation is the first column
  for (const std::vector<std::string>& row : data) {
    if (row[0] == abbreviation) {
      return row;
    }
  }
  std::cerr << "Error: Abbreviation " << abbreviation << " not found" << std::endl;
  return {};
}