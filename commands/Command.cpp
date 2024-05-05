#include "Command.hpp"
#include "../game/Game.hpp"

bool HelpCommand::execute()
{
  std::cout << "help" << std::endl;
  return true;
}

bool QuitCommand::execute()
{
    std::cout << "quit" << std::endl;

  game_->toggleGame();

  return true;
}