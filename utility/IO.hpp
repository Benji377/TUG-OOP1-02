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
#include <memory>
#include <map>

using std::iostream;
using std::fstream;
using std::string;

class Entity;
class Player;
class Inventory;
class Item;
class Room;
class Character;
class Dice;

enum class DamageType;

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
  /// @param check_input If true, the function will check if the input is "quit" and return "quit" in that case.
  ///
  /// @return A vector of strings containing the input tokens.
  //
  static std::string promtUserInput(bool check_input = true);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// This function takes a string as input and splits it into a vector of words,
  /// using a specified delimiter (default is whitespace).
  ///
  /// @param input The string to be split.
  ///
  /// @return A vector of strings containing the words from the input string.
  //
  static std::vector<std::string> commandifyString(std::string input);

  static void printPlayerPosition(std::shared_ptr<Player> player, std::shared_ptr<Room> room);

  static void printEnemyPosition(std::map<std::string, std::shared_ptr<Character>> enemies_mapped,
  std::shared_ptr<Room>  current_room);

  static void printPlayerInventory(std::shared_ptr<Player> player);

  static void printInventory(std::shared_ptr<Inventory> inv, std::shared_ptr<Player> player);

  //Using the fact that a map will always be sorted alphabetically
  static void printVectorOfItemsAlphabetically(std::vector<std::shared_ptr<Item>> items);

  static void printActives(std::shared_ptr<Player> player);

  static void printPlayerMoved(std::shared_ptr<Player> player, std::pair<int,int> position);

  static void printDamageTypeResistance(DamageType type);

  static void printSuccessFullAttack(std::shared_ptr<Player> player, std::pair<int, int>& target_position,
    std::vector<std::vector<int>>& affected_fields);

  friend std::ostream& operator<<(std::ostream& os, std::pair<int,int>& pair);

  static void printDiceRoll(int result, std::shared_ptr<Dice> dice);

  static void printAttackedCharacters(std::vector<struct AttackedCharacter> characters);



};



#endif