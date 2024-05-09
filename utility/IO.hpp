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

#include "Utils.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>

class Entity;
class Player;
class Room;
class Character;


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
  static std::string promtUserInput();

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Converts a given string into a vector of individual words delimited by spaces.
  /// 
  /// @param input The input string to be converted.
  ///
  /// @return A vector containing individual words extracted from the input string.
  ///
  static std::vector<std::string> commandifyString(std::string input);

  //----  -----------------------------------------------------------------------------------------------------------------
  ///
  /// Prints the position of a player within a room.
  ///
  /// @param player A shared pointer to the player object whose position is to be printed.
  /// @param room A shared pointer to the room object containing the player.
  ///
  static void printPlayerPosition(std::shared_ptr<Player> player, std::shared_ptr<Room> room);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Prints the position of each enemy within a given room.
  ///
  /// @param enemies_mapped A map containing enemy names as keys and shared pointers to character objects as values.
  /// @param current_room A shared pointer to the room object containing the enemies.
  ///
  static void printEnemyPosition(std::map<std::string, std::shared_ptr<Character>> enemies_mapped,
  std::shared_ptr<Room>  current_room);

static int getDigitAmount(int number);

  static void printPlayerStats(std::shared_ptr<Player> player_ptr);

};



#endif