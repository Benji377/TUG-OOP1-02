//---------------------------------------------------------------------------------------------------------------------
//
// Input / Output Class. Handles all user in and output. Functions are declared static, so you don't need an object
// to call them.
//
// Group: 068
//
// Author: Tutors, Hanno Elsnig
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef IO_HPP
#define IO_HPP

#include "../utility/Utils.hpp"
#include <string>
#include <fstream>
#include <iostream>

class IO
{
public:
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
  static bool checkMagicNumber(char* config_path, std::string magic_number);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Prompts the user for input with a ">" symbol and returns a vector of strings containing the input tokens.
  ///
  /// @return A vector of strings containing the input tokens.
  //
  static std::vector<std::string> promtUserInput();

};



#endif