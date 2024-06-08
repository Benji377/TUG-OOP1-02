//------------------------------------------------------------------------------------------------------------------
//
// Command class: These are the different types of commands. There is an abstract base class whose execute method
// is overloaded depending on the type of command.
//
// Group: 068
//
// Author: Hanno Elsnig, 12303626
//---------------------------------------------------------------------------------------------------------------------
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../utility/Exceptions.hpp"
#include "../dungeon/Room.hpp"
#include <cctype>
#include <memory>
#include <vector>

using std::string;
using std::iostream;
class Game;
class Player;
class Door;
class DeathLocation;
class TreasureChest;

enum class Abbrev {
    PLAYER,
    ITEM,
};

class Command
{
protected:
  //All commands except for the help command need the game. So I just added it for all of them
  Game* game_;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor is set to protected so this class can only be initiated by subclasses.
  //
  Command(Game* game) : game_(game) {};

  //--------------------------------------------------------------------------------------------------------------------
  ///
  /// Checks if the number of parameters matches the required size and throws an exception if not.
  ///
  /// @param params The vector of strings representing the parameters.
  /// @param required_size The required number of parameters.
  ///
  /// @throws std::invalid_argument if the number of parameters doesn't match the required size.
  //
  void checkCommandLenght(std::vector<std::string> params, size_t required_size) const;

  //--------------------------------------------------------------------------------------------------------------------
  /// This function searches for a player with the specified abbreviation in the game's list of players.
  /// If found, it returns a shared pointer to the player. If no player with the specified abbreviation
  /// is found, it throws an InvalidParamCommand exception.
  ///
  /// @param params The vector of strings representing the parameters.
  /// @param position_of_abbrev_in_params The index of the abbreviation in the params vector.
  ///
  /// @return A shared pointer to the player with the specified abbreviation.
  ///
  /// @throws InvalidParamCommand if no player with the specified abbreviation is found.
  //
  std::shared_ptr<Player> getPlayerOfAbbrev(std::vector<std::string> params, size_t position_of_abbrev_in_params) const;

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Parses a position string formatted as "x,y" into a vector of integers.
  ///
  /// @param position_string The position string to parse.
  /// @return A vector of integers containing the parsed position.
  /// @throws InvalidParamCommand if the string cannot be parsed into exactly two integers.
  //
  std::vector<int> getPositionAsVecOutOfString(std::string position_string) const;

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Retrieves a player and their adjacent target position from the parameters vector.
  ///
  /// @param params The vector of parameters, which should include the player abbreviation and the target position.
  /// @return A pair containing a shared pointer to the Player object and a pair of integers representing the target position.
  /// @throws UnavailableItemOrEntityCommand if the player is not found.
  /// @throws InvalidPositionCommand if the target position is not adjacent to the player's current position.
  //
  std::pair<std::shared_ptr<Player>, std::pair<int, int>> getPlayerAndAdjacentField(std::vector<std::string> params);

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Parses a position string formatted as "x,y" into a pair of integers.
  ///
  /// @param position The position string to parse.
  /// @return A pair of integers containing the parsed position.
  /// @throws InvalidParamCommand if the string cannot be parsed into exactly two integers.
  //
  std::pair<int,int> getPositionAsPairOutOfString(std::string position) const;



public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Destructor is set to empty, can be overriden tho, maybe we need memory at some point?
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  virtual ~Command() {}
  Command(const Command&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// execute function that differes from subclass to subclass. This is where the actual command logic happens
  /// @return void
  ///
  //------------------------------------------------------------------------------------------------------------------
  virtual void execute(std::vector<std::string> params) = 0;


};


#endif
