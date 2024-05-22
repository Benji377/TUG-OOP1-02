//---------------------------------------------------------------------------------------------------------------------
//
// Input / Output Class. Handles most of user in and output. Functions are declared static, so you don't need an object
// to call them.
//
// Group: 068
//
// Author: Hanno Elsnig, 12303626
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef INPUTOUTPUT_HPP
#define INPUTOUTPUT_HPP

#include "Utils.hpp"
#include "../entity/character/Enemy.hpp"
#include "../dungeon/AttackedField.hpp"
#include <memory>
#include <map>

class Entity;
class Player;
class Inventory;
class Item;
class Room;
class Character;
class Dice;

enum class DamageType;

class InputOutput
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

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Prints the position of a player within a room.
  ///
  /// @param player A shared pointer to the Player object.
  /// @param room A shared pointer to the Room object.
  //
  static void printPlayerPosition(std::shared_ptr<Player> player, std::shared_ptr<Room> room);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Prints the positions of enemies within the current room.
  ///
  /// @param enemies_mapped A map of enemy names to shared pointers of Character objects.
  /// @param current_room A shared pointer to the current Room object.
  //
  static void printEnemyPosition(std::map<std::string, std::shared_ptr<Character>> enemies_mapped,
  std::shared_ptr<Room>  current_room);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Prints the full inventory of a player, including active equipment and other items.
  ///
  /// @param player A shared pointer to the Player object.
  //
  static void printPlayerInventory(std::shared_ptr<Player> player);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Prints the inventory of a player, excluding currently equipped items. 
  ///
  /// @param inv A shared pointer to the Inventory object.
  /// @param player A shared pointer to the Player object, used to exclude equipped items from printing.
  //
  static void printInventory(std::shared_ptr<Inventory> inv, std::shared_ptr<Player> player);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Using the fact that a map will always be sorted alphabetically:
  /// Prints a vector of items in alphabetical order, including their abbreviations and quantities.
  ///
  /// @param items A vector of shared pointers to Item objects.
  //
  static void printVectorOfItemsAlphabetically(std::vector<std::shared_ptr<Item>> items);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Prints the active items of a player = equipped armor and weapon.
  ///
  /// @param player A shared pointer to the Player object.
  //
  static void printActives(std::shared_ptr<Player> player);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Prints a message indicating that a player has moved to a new position.
  ///
  /// @param player A shared pointer to the Player object.
  /// @param position A pair of integers representing the new position.
  //
  static void printPlayerMoved(std::shared_ptr<Player> player, std::pair<int,int> position);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Prints a message indicating that a character has become resistant to a specific type of damage.
  ///
  /// @param type The type of damage (e.g., FIRE, ACID).
  //
  static void printDamageTypeResistance(DamageType type);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Prints a message indicating the success of an attack, including the attacker's name, weapon used,
  /// target position, and affected fields.
  ///
  /// @param attacker A shared pointer to the Character object representing the attacker.
  /// @param target_position A pair of integers representing the target position.
  /// @param attacked_fields A vector of AttackedField objects representing the fields affected by the attack.
  //
  static void printSuccessFullAttack(std::shared_ptr<Character> attacker, std::pair<int, int>& target_position,
    std::vector<AttackedField>& affected_characters);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Prints the result of a dice roll, including the type and amount of dice rolled.
  ///
  /// @param result The result of the dice roll.
  /// @param dice A shared pointer to the Dice object.
  //
  static void printDiceRoll(int result, std::shared_ptr<Dice> dice);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Prints information about characters that were attacked, including whether they were defeated.
  ///
  /// @param attacked_fields A vector of AttackedField objects representing the fields that were attacked.
  //
  static void printAttackedCharacters(std::vector<AttackedField> characters);

};

#endif