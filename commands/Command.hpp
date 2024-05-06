//------------------------------------------------------------------------------------------------------------------
///
/// Command class: These are the different types of commands. There is an abstract base class whose execute method
/// is overloaded depending on the type of command.
///
// Group: 068
//
// Author: Hanno Elsnig
//------------------------------------------------------------------------------------------------------------------
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iostream>
#include <vector>

class Game;

class Command
{
protected:
  //All commands except for the help command need the game. So I just added it for all of them
  Game* game_;
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor is set to protected so this class can only be initiated by subclasses.
  ///
  //------------------------------------------------------------------------------------------------------------------
  Command(Game* game) : game_(game) {};

  //---------------------------------------------------------------------------------------------------------------------
  ///
  /// Checks if the number of parameters matches the required size and throws an exception if not.
  ///
  /// @param params The vector of strings representing the parameters.
  /// @param required_size The required number of parameters.
  ///
  /// @throws std::invalid_argument if the number of parameters doesn't match the required size.
  //
  void checkParameterCount(std::vector<std::string> params, size_t required_size);

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




//------------------------------------------------------------------------------------------------------------------
///
/// HelpCommand class: This class represents a command that provides help information.
///
//------------------------------------------------------------------------------------------------------------------
class HelpCommand: public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Help class calls Constructor of command class. As game ptr is not needed for help, it initialises
  /// the game for this to the nullptr.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  HelpCommand() : Command(NULL) {};
  virtual ~HelpCommand() {};
  HelpCommand(const HelpCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// then prints the help text
  ///
  //------------------------------------------------------------------------------------------------------------------
  virtual void execute(std::vector<std::string> params) override;
};






//------------------------------------------------------------------------------------------------------------------
///
/// MapCommand class: This class represents a command that toggles output of map in the game. 
/// Gets initialised with the address of the game.
///
//------------------------------------------------------------------------------------------------------------------
class MapCommand : public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Map class calls Constructor of command class, which initialises the game ptr.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  MapCommand(Game* game) : Command(game) {}
  virtual ~MapCommand() {};
  MapCommand(const MapCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// Then toggles the map output in the game
  ///
  //------------------------------------------------------------------------------------------------------------------
  virtual void execute(std::vector<std::string> params) override;
};




//------------------------------------------------------------------------------------------------------------------
///
/// QuitCommand class: This class represents a command that quits the game. Gets initialised with the address of the
/// game.
///
//------------------------------------------------------------------------------------------------------------------
class QuitCommand : public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Quit class calls Constructor of command class, which initialises the game ptr.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  QuitCommand(Game* game) : Command(game) {}
  virtual ~QuitCommand() {};
  QuitCommand(const QuitCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// Then toggles the is_active_ status of the game
  ///
  //------------------------------------------------------------------------------------------------------------------
  virtual void execute(std::vector<std::string> params) override;
};




//------------------------------------------------------------------------------------------------------------------
///
/// PositionsCommand class: This class represents a command prints the positions of enemies and players. 
//  Gets initialised with the address of the game.
///
//------------------------------------------------------------------------------------------------------------------
class PositionsCommand : public Command
{
public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor of Positions class calls Constructor of command class, which initialises the game ptr.
  /// Destructor same as abstract class.
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  PositionsCommand(Game* game) : Command(game) {}
  virtual ~PositionsCommand() {};
  PositionsCommand(const PositionsCommand&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute method that overrides the abstract classes execute. First checks whether the parameters match
  /// Then prints the positions of the players.
  ///
  //------------------------------------------------------------------------------------------------------------------
  virtual void execute(std::vector<std::string> params) override;
};


#endif
