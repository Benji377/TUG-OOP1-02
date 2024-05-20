#include "Utils.hpp"
#include "Exceptions.hpp"
#include "../entity/character/Inventory.hpp"

bool Utils::decimalStringToInt(const std::string& str, int& number)
{
  size_t position = 0;
  try
  {
    number = std::stoul(str, &position, 10);
  }
  catch (std::exception&)
  {
    return false;
  }
  return position == str.length();
}


std::fstream& Utils::goToLine(std::fstream& file, int num)
{
  file.seekg(std::ios::beg);
  for(int i=0; i < num - 1; ++i)
  {
    file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
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

void Utils::isValidConfig(const char *config_path, const char* magic_number)
{
  std::fstream config_file(config_path);
  // We need to check if we can open the file first, else we would get an error when reading from it
  if(!config_file.is_open())
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

std::vector<std::string> Utils::splitString(const std::string& string, const std::string& delimiter)
{
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = string.find(delimiter, pos_start)) != std::string::npos)
  {
    token = string.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back (token);
  }
  res.push_back(string.substr (pos_start));
  return res;
}

void Utils::normalizeString(std::string& string, bool to_upper)
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

std::vector<char> Utils::getDifference(const std::vector<char>& a, const std::vector<char>& b)
{
    std::vector<char> difference;
    for (const auto& item : a)
    {
        if (std::find(b.begin(), b.end(), item) == b.end())
        {
            difference.push_back(item);
        }
    }
    return difference;
}

void Utils::deleteDuplicates(std::vector<char>& vec)
{
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

bool Utils::isValidItemAbbrev(std::string abbrev)
{
  std::shared_ptr<Inventory> test_inventory = std::make_shared<Inventory>();
  std::transform(abbrev.begin(), abbrev.end(), abbrev.begin(), ::toupper);
  std::map<std::string, int> parse_input;
  parse_input.insert(make_pair(abbrev, 1));
  test_inventory->parseInventory(parse_input, 0, 0);

  if(test_inventory->getAllPotions().empty() && test_inventory->getAllArmor().empty()
      && test_inventory->getAllWeapons().empty())
    {
      return false;
    }
    return true;
}