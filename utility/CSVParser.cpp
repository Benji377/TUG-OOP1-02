#include "CSVParser.hpp"

const std::filesystem::path CSVParser::cwd = std::filesystem::current_path();
const std::string CSVParser::armor_csv_path_ = "csv/armor.csv";
const std::string CSVParser::potions_csv_path_ = "csv/potions.csv";
const std::string CSVParser::weapon_csv_path_ =  "csv/weapons.csv";
const std::string CSVParser::ammunition_csv_path_ = "csv/ammunition.csv";

std::vector<std::vector<std::string>> CSVParser::readCSV(const std::string& filename) {
  std::vector<std::vector<std::string>> data;
  std::ifstream file(filename);
  // Check if file exists
  if (!file.good()) {
    std::cout << "Error: File " << filename << " does not exist" << std::endl;
    return data;
  }
  if (!file.is_open()) {
    std::cout << "Error: Could not open file " << filename << std::endl;
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

std::string CSVParser::getFolderByAbbreviation(const std::string& abbreviation)
{
  // Map the filenames to the corresponding item types
  std::map<std::string, std::string> filenameToItemType = {
          {armor_csv_path_, "Armor"},
          {potions_csv_path_, "Potion"},
          {weapon_csv_path_, "Weapon"},
          {ammunition_csv_path_, "Ammunition"}
  };

  // Iterate through each CSV file in the items/csv folder
  for (const auto& [filename, itemType] : filenameToItemType)
  {
    std::vector<std::string> row = getRowByAbbreviation(filename, abbreviation);
    if (!row.empty())
    {
      return itemType;
    }
  }
  // If the abbreviation was not found in any of the files, return an empty string
  return "";
}