#include "Utils.hpp"
#include "Exceptions.hpp"
#include "../entity/character/Inventory.hpp"

bool Utils::decimalStringToInt(const std::string &str, int &number)
{
  size_t position = 0;
  try
  {
    number = std::stoul(str, &position, 10);
  }
  catch (std::exception &)
  {
    return false;
  }
  return position == str.length();
}


std::fstream &Utils::goToLine(std::fstream &file, int line_number)
{
  file.seekg(std::ios::beg);
  for (int current_line = 0; current_line < line_number - 1; ++current_line)
  {
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return file;
}

std::string Utils::readFileLine(const char *config_path, int line_number)
{
  std::fstream target_file(config_path);
  // Check if the file is open
  if (!target_file.is_open())
  {
    throw std::runtime_error("[UTILS] Unable to open file");
  }
  // Move the file pointer to the specified line
  goToLine(target_file, line_number);
  // Check if the line number is within the range of the file's lines
  if (target_file.eof())
  {
    throw std::out_of_range("[UTILS] Line number out of range");
  }
  std::string output;
  std::getline(target_file, output);
  return output;
}

void Utils::isValidConfig(const char *config_path, const char *magic_number)
{
  std::fstream config_file(config_path);
  // We need to check if we can open the file first, else we would get an error when reading from it
  if (!config_file.is_open())
  {
    throw InvalidConfigFileException(config_path);
  }
  // Now we can read from the file and check if the first line is "OOP"
  std::string magic = readFileLine(config_path, 0);
  if (magic == magic_number)
  {
    return;
  }
  throw InvalidConfigFileException(config_path);
}

std::vector<std::string> Utils::splitString(const std::string &string, const std::string &delimiter)
{
  size_t pos_start = 0;
  size_t delimiter_length = delimiter.length();
  size_t pos_end;
  std::string token;
  std::vector<std::string> split_string_vector;

  while ((pos_end = string.find(delimiter, pos_start)) != std::string::npos)
  {
    token = string.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delimiter_length;
    split_string_vector.push_back(token);
  }
  split_string_vector.push_back(string.substr(pos_start));
  return split_string_vector;
}

void Utils::normalizeString(std::string &string, bool to_upper)
{
  // Remove trailing whitespaces
  size_t end = string.find_last_not_of(' ');
  string = end == std::string::npos ? "" : string.substr(0, end + 1);
  // Remove leading whitespaces
  auto start = string.find_first_not_of(' ');
  string = start == std::string::npos ? "" : string.substr(start);
  // Transform all characters to lowercase
  if (to_upper)
    std::transform(string.begin(), string.end(), string.begin(), ::toupper);
  else
    std::transform(string.begin(), string.end(), string.begin(), ::tolower);
}

std::vector<char> Utils::getDifference(const std::vector<char> &first_vector, const std::vector<char> &second_vector)
{
  std::vector<char> difference;
  for (const auto &item: first_vector)
  {
    if (std::find(second_vector.begin(), second_vector.end(), item) == second_vector.end())
    {
      difference.push_back(item);
    }
  }
  return difference;
}

void Utils::deleteDuplicates(std::vector<char> &char_vector)
{
  std::sort(char_vector.begin(), char_vector.end());
  char_vector.erase(std::unique(char_vector.begin(), char_vector.end()), char_vector.end());
}

bool Utils::isValidItemAbbrev(std::string item_abbreviation)
{
  std::shared_ptr<Inventory> test_inventory = std::make_shared<Inventory>();
  std::transform(item_abbreviation.begin(), item_abbreviation.end(), item_abbreviation.begin(), ::toupper);
  std::map<std::string, int> parse_input;
  parse_input.insert(make_pair(item_abbreviation, 1));
  test_inventory->parseInventory(parse_input, 'X', 0, 0);

  if (test_inventory->getAllPotions().empty() && test_inventory->getAllArmor().empty()
      && test_inventory->getAllWeapons().empty())
  {
    return false;
  }
  return true;
}