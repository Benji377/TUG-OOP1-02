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
  /// @throws std::runtime_error if the number of parameters doesn't match the required size.
  //
  void checkParameterCount(std::vector<std::string> params, size_t required_size);

public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Destructor is set to default, can be overriden tho, maybe we need memory at some point?
  /// This class should never be copied. Each command type only exists once.
  ///
  //------------------------------------------------------------------------------------------------------------------
  virtual ~Command() {}
  Command(const Command&) = delete;

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// execute function that differes from subclass to subclass. This is where the actual command logic happens
  /// @return bool whether the command was successfull or not
  ///
  //------------------------------------------------------------------------------------------------------------------
  virtual bool execute(std::vector<std::string> params) = 0;

};

//------------------------------------------------------------------------------------------------------------------
///
/// HelpCommand class: This class represents a command that provides help information.
///
//------------------------------------------------------------------------------------------------------------------
class HelpCommand: public Command
{
public:
  HelpCommand() : Command(NULL) {};
  virtual ~HelpCommand() {};
  HelpCommand(const HelpCommand&) = delete;

  virtual bool execute(std::vector<std::string> params) override;
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
  QuitCommand(Game* game) : Command(game) {}
  virtual ~QuitCommand() {};
  QuitCommand(const QuitCommand&) = delete;

  virtual bool execute(std::vector<std::string> params) override;
};


#endif
