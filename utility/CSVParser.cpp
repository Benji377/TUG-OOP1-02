#include "CSVParser.hpp"

const std::string CSVParser::ARMOR_CSV_PATH = "data/armor.csv";
const std::string CSVParser::POTIONS_CSV_PATH = "data/potions.csv";
const std::string CSVParser::WEAPON_CSV_PATH = "data/weapons.csv";
const std::string CSVParser::AMMUNITION_CSV_PATH = "data/ammunition.csv";

bool checkFile(const std::string &filename)
{
  try
  {
    std::ifstream file(filename);
    if (!file.good())
    {
      throw std::runtime_error("File " + filename + " does not exist");
    }
    if (!file.is_open())
    {
      throw std::runtime_error("Could not open file " + filename);
    }
    file.close();
    return true;
  }
  catch (const std::exception &e)
  {
    std::cout << "[CSVPARSER] Error: " << e.what() << std::endl;
    return false;
  }
}

std::vector<std::vector<std::string>> CSVParser::readCSV(const std::string &filename)
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
    // Equivalent to an empty file. We don't want to throw an exception here as the file might be willingly empty
    // The check for empty file is done in the calling function
    return {};
  }
}

std::vector<std::string> CSVParser::getRowByAbbreviation(const std::string &filename, const std::string &abbreviation)
{
  std::vector<std::vector<std::string>> data = readCSV(filename);
  if (data.empty())
  {
    return {};
  }
  // The abbreviation is the first column
  for (const std::vector<std::string> &row: data)
  {
    if (row[0] == abbreviation)
    {
      return row;
    }
  }
  return {};
}

std::string CSVParser::getFolderByAbbreviation(const std::string &abbreviation)
{
  // Map the filenames to the corresponding item types
  std::map<std::string, std::string> filename_to_item_type = {
          {ARMOR_CSV_PATH,       "Armor"},
          {POTIONS_CSV_PATH,     "Potion"},
          {WEAPON_CSV_PATH,      "Weapon"},
          {AMMUNITION_CSV_PATH, "Ammunition"}
  };

  // Iterate through each CSV file in the data folder
  for (const auto &[filename, itemType]: filename_to_item_type)
  {
    std::vector<std::string> row = getRowByAbbreviation(filename, abbreviation);
    if (!row.empty())
    {
      return itemType;
    }
  }
  // Equivalent to an item not being found. This can happen and is handled in the calling function
  return "";
}