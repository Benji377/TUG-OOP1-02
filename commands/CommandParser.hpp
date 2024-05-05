#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include <map>
#include <string>
#include <memory>

#include "Command.hpp"

class CommandParser
{
  private:
  std::map<std::string, Command*> commands_;

  public:
  ~CommandParser();
  CommandParser(CommandParser&) = delete;
  CommandParser() {}

  void registerCommand(const std::string name, Command* command);
  bool execute(std::string input);


};



#endif