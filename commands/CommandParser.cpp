#include "CommandParser.hpp"

void CommandParser::registerCommand(const std::string name, std::unique_ptr<Command> command)
{
  commands_.insert({name, std::move(command)});
}

bool CommandParser::execute(std::vector<std::string> user_input)
{
  if (commands_.count(user_input.at(0)) == 0)
  {
    throw std::invalid_argument("UnknownCommand");
  }

  commands_.at(user_input.at(0))->execute(user_input);
  return true;
}

CommandParser::~CommandParser()
{}