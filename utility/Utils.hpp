//---------------------------------------------------------------------------------------------------------------------
//
// The Utils class contains some useful functions for use in other classes.
// It has been extracted from A1 and adapted with further functionality for use in A2.
//
// Author: Tutors
//         Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <limits>
#include <algorithm>
#include <map>

class Utils
{
  private:
    ///------------------------------------------------------------------------------------------------------------------
    ///
    /// Utils class extension: This function moves the file pointer to a specific line in a file.
    /// This is a helper function for the readConfigLine function.
    ///
    /// @param file The file to be read
    /// @param line_number The line number to be read
    ///
    /// @return The file with the file pointer at the specified line
    //
    static std::fstream &goToLine(std::fstream &file, int line_number);
  public:
    ///------------------------------------------------------------------------------------------------------------------
    /// Constructor, Copy-Constructor and Destructor are deleted, as this class should never be instantiated.
    //
    Utils() = delete;
    Utils(const Utils &) = delete;
    ~Utils() = delete;

    ///------------------------------------------------------------------------------------------------------------------
    /// This function converts a string into an integer. The conversion is only successful, if all
    /// elements of the string are converted.
    ///
    /// @param str - string that should be converted
    /// @param number - the converted number
    ///
    /// @return true, if conversion was successful, false otherwise
    //
    static bool decimalStringToInt(const std::string &str, int &number);
    ///------------------------------------------------------------------------------------------------------------------
    ///
    /// Utils class extension: This function reads a specific line from a file. The line number is given as a parameter.
    ///
    /// @param file_path The path to the file
    /// @param line_number The line number to be read
    ///
    /// @return The line read from the file
    //
    static std::string readFileLine(const char *file_path, int line_number);
    ///------------------------------------------------------------------------------------------------------------------
    ///
    /// Utils class extension: This function checks if a given configuration file is valid. A configuration file is valid
    /// if it contains the correct magic number at the start of the file. If the file is not valid, an exception is thrown.
    ///
    /// @param config_path The path to the configuration file
    /// @param magic_number The magic number to be checked
    //
    static void isValidConfig(const char *config_path, const char *magic_number);

    ///------------------------------------------------------------------------------------------------------------------
    ///
    /// Utils class extension: This function splits a string into a vector of strings using a delimiter.
    ///
    /// @param string The string to be split
    /// @param delimiter The delimiter to split the string
    ///
    /// @return A vector of strings
    //
    static std::vector<std::string> splitString(const std::string &string, const std::string &delimiter);
    ///------------------------------------------------------------------------------------------------------------------
    ///
    /// Utils class extension: This function normalizes a string by removing all whitespace and converting it by default
    /// to lowercase. If the to_upper parameter is set to true, the string is converted to uppercase.
    ///
    /// @param string The string to be normalized
    /// @param to_upper True if the string should be converted to uppercase, false otherwise
    //
    static void normalizeString(std::string &string, bool to_upper = false);
    ///------------------------------------------------------------------------------------------------------------------
    ///
    /// Utils class extension: This function checks if a given object is an instance of a specific class.
    ///
    /// @param ptr The object to be checked
    ///
    /// @return True if the object is an instance of the class, false otherwise
    //
    template<typename Base, typename T>
      static bool instanceof(const T *ptr)
      {
        return dynamic_cast<const Base *>(ptr) != nullptr;
      }
    ///------------------------------------------------------------------------------------------------------------------
    ///
    /// Utils class extension: This function returns the difference between two vectors. The difference is calculated by
    /// comparing the elements of the first vector with the elements of the second vector. If an element is not found in
    /// the second vector, it is added to the result vector.
    ///
    /// @param first_vector The first vector
    /// @param second_vector The second vector
    ///
    /// @return A vector containing the difference between the two input vectors
    //
    static std::vector<char> getDifference(const std::vector<char> &first_vector, const std::vector<char> &second_vector);
    ///------------------------------------------------------------------------------------------------------------------
    ///
    /// Utils class extension: This function deletes duplicates from a vector. The function iterates over the vector and
    /// removes all duplicates. The vector is modified in place.
    ///
    /// @param char_vector The vector to be modified
    //
    static void deleteDuplicates(std::vector<char> &char_vector);
    ///------------------------------------------------------------------------------------------------------------------
    ///
    /// Utils class extension: This function checks if a given item abbreviation is valid. An item abbreviation is valid
    /// if it is contained in the item abbreviation map of the inventory.
    ///
    /// @param item_abbreviation The item abbreviation to be checked
    ///
    /// @return True if the item abbreviation is valid, false otherwise
    //
    static bool isValidItemAbbrev(std::string item_abbreviation);

    static std::pair<int, int> getClosestPosition(const std::pair<int, int> current_position,
                                                  const std::vector<std::vector<int>> positions);

};

#endif // UTILS_HPP