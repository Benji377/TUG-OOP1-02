#include "CommandParser.hpp"

//Idea for this from OO KU Practicals 5
void CommandParser::registerCommand(const std::string name, std::unique_ptr<Command> command)
{
  commands_.insert({name, std::move(command)});
}

bool CommandParser::execute(std::string input)
{
  if (commands_.count(input) == 0)
  {
    return false;
  }

  commands_.at(input)->execute();
  return true;
}

//Idea for this from OO KU Practicals 5
CommandParser::~CommandParser()
{}