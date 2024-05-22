//------------------------------------------------------------------------------------------------------------------
//
// CommandParser class: This class provides functionality to register and execute commands.
// It stores a map of string keys that map to unique pointers to the different commands that exist
// The idea for this type of implementation comes from the Practicals of OOP Week 5
//
// Group: 068
//
// Author: Tutors KU OOP, Hanno Elsnig 12303626
//------------------------------------------------------------------------------------------------------------------
#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include <memory>
#include <map>
#include <vector>

#include "Command.hpp"
#include "AttackCommand.hpp"
#include "HelpCommand.hpp"
#include "InventoryCommand.hpp"
#include "LootCommand.hpp"
#include "MapCommand.hpp"
#include "MoveCommand.hpp"
#include "PlayerCommand.hpp"
#include "PositionsCommand.hpp"
#include "QuitCommand.hpp"
#include "StoryCommand.hpp"
#include "UseCommand.hpp"

class CommandParser
{
private:
  std::map<std::string, std::unique_ptr<Command>> commands_;

public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructer is set to default. Destructor is set to empty as no extra memory handling is necessary. 
  /// This class should never be copied.
  ///
  //------------------------------------------------------------------------------------------------------------------
  ~CommandParser();
  CommandParser(CommandParser&) = delete;
  CommandParser() {}

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Register a command with a given name and command object
  ///
  /// @param name The name of the command
  /// @param command The command object to be associated with the name
  //------------------------------------------------------------------------------------------------------------------
  void registerCommand(const std::string name, std::unique_ptr<Command> command);

  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Execute the command associated with the given input. Goes through the map and calls the execute method of the
  /// Command if it exists.
  ///
  /// @param user_input The input string representing the command to be executed
  ///
  /// @return true if the command is executed successfully, false otherwise
  //
  bool execute(std::vector<std::string> user_input);

};



#endif