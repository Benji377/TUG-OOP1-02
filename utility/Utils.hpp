//---------------------------------------------------------------------------------------------------------------------
//
// The Utils class contains some useful functions for use in other classes.
//
// Author: Tutors
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

class Utils
{
public:
  //------------------------------------------------------------------------------------------------------------------
  // Constructor, Copy-Constructor and Destructor are deleted, as this class should never be instantiated.
  //
  Utils() = delete;
  Utils(const Utils&) = delete;
  ~Utils() = delete;

  //------------------------------------------------------------------------------------------------------------------
  // This function converts a string into an integer. The conversion is only successful, if all
  // elements of the string are converted.
  //
  // @param str - string that should be converted
  // @param number - the converted number
  // @return true, if conversion was successful, false otherwise
  //
  static bool decimalStringToInt(const std::string& str, int& number);

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Utils class extension: This function reads a specific line from a file. The line number is given as a parameter.
  ///
  /// @param config_path The path to the file
  /// @param line_number The line number to be read
  ///
  /// @return The line read from the file
  //
  static std::string readConfigLine(char* config_path, int line_number);

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Utils class extension: This function checks if a given configuration file is valid. A configuration file is valid
  /// if it contains the correct magic number at the start of the file.
  ///
  /// @param config_path The path to the file
  ///
  /// @return true if the configuration file is valid, false otherwise
  //
  static bool isValidConfig(char* config_path);

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Utils class extension: This function splits a string into a vector of strings using a delimiter.
  ///
  /// @param string The string to be split
  /// @param delimiter The delimiter to split the string
  ///
  /// @return A vector of strings
  //
  static std::vector<std::string> splitString(const std::string& string, const std::string& delimiter);
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Utils class extension: This function normalizes a string by removing leading and trailing whitespaces.
  /// It furthermore converts all characters to lowercase.
  ///
  /// @param string The string to be normalized
  //
  static void normalizeString(std::string& string);

private:
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Utils class extension: This function moves the file pointer to a specific line in a file.
  /// This is a helper function for the readConfigLine function.
  ///
  /// @param file The file to be read
  /// @param num The line number to be read
  ///
  /// @return The file with the file pointer at the specified line
  //
  static std::fstream& goToLine(std::fstream& file, int num);

};

#endif // UTILS_HPP

