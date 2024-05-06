#include "IO.hpp"

//---------------------------------------------------------------------------------------------------------------------
///
/// Checks if the first line of a file matches a specified magic number. It is declared "static" in the .hpp file
/// So you don't have to create an instance of IO for the function to work.
///
/// @param config_path Path to the configuration file.
/// @param magic_number The magic number to compare against.
///
/// @return true if the first line of the file matches the magic number, false otherwise or if the file cannot be opened.
//
bool IO::checkMagicNumber(char* config_path, std::string magic_number)
{
  std::string filename{config_path};
  std::ifstream file(filename);
  if (!file.is_open())
  {
    return false;
  }

  std::string first_line_of_file;
  std::getline(file, first_line_of_file);

  if(first_line_of_file == magic_number)
  {
    return true;
  }

  return false;
}