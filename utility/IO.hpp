//---------------------------------------------------------------------------------------------------------------------
//
// Input / Output Class. Handles all user in and output
//
// Group: 068
//
// Author:
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef IO_HPP
#define IO_HPP

#include <string>
#include <fstream>
#include <iostream>



class IO
{
  public:
  static bool checkMagicNumber(char* config_path, std::string magic_number);

};



#endif