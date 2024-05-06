//------------------------------------------------------------------------------------------------------------------
///
/// CommandParser class: This class provides functionality to register and execute commands.
/// It stores a map of string keys that map to unique pointers to the different commands that exist
/// The idea for this type of implementation comes from the Practicals of OOP Week 5
///
// Group: 068
//
// Author: Hanno Elsnig
//------------------------------------------------------------------------------------------------------------------
#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include <map>
#include <string>
#include <memory>
#include <vector>

#include "Command.hpp"

class CommandParser
{
private:
  std::map<std::string, std::unique_ptr<Command>> commands_;

public:
  //------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructer and Destructor are set to default. This class should never be copied.
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
  /// @param input The input string representing the command to be executed
  ///
  /// @return true if the command is executed successfully, false otherwise
  //
  bool execute(std::vector<std::string> input);

};



#endif