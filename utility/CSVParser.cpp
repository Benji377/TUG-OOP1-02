#include "CSVParser.hpp"

const std::string CSVParser::armor_csv_path_ = "data/armor.csv";
const std::string CSVParser::potions_csv_path_ = "data/potions.csv";
const std::string CSVParser::weapon_csv_path_ = "data/weapons.csv";
const std::string CSVParser::ammunition_csv_path_ = "data/ammunition.csv";

bool checkFile(const std::string& filename)
{
  try
  {
    std::ifstream file(filename);
    if (!file.good())
    {
      throw std::runtime_error("Error: File " + filename + " does not exist");
    }
    if (!file.is_open())
    {
      throw std::runtime_error("Error: Could not open file " + filename);
    }
    file.close();
    return true;
  }
  catch (const std::exception& e)
  {
    std::cout << "[CSVPARSER] Error: " << e.what() << std::endl;
    return false;
  }
}

std::vector<std::vector<std::string>> CSVParser::readCSV(const std::string& filename)
{
  std::vector<std::vector<std::string>> data;
  if (checkFile(filename))
  {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
      std::vector<std::string> row;
      std::stringstream ss(line);
      std::string cell;
      while (std::getline(ss, cell, ';'))
      {
        row.push_back(cell);
      }
      data.push_back(row);
    }
    file.close();
    return data;
  }
  else
  {
    return {};
  }
}

std::vector<std::string> CSVParser::getRowByAbbreviation(const std::string& filename, const std::string& abbreviation)
{
  std::vector<std::vector<std::string>> data = readCSV(filename);
  if (data.empty())
  {
    return {};
  }
  // The abbreviation is the first column
  for (const std::vector<std::string>& row : data)
  {
    if (row[0] == abbreviation)
    {
      return row;
    }
  }
  std::cout << "[CSVPARSER] Error: Abbreviation " << abbreviation << " not found in file " << filename << std::endl;
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
  std::cout << "[CSVPARSER] Error: Abbreviation " << abbreviation << " not found in any file" << std::endl;
  return "";
}