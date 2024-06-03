#include "Decision.hpp"

std::string Decision::getBestCommand()
{
  std::map<double, std::string> best_commands;
  for(auto player : game_->getLivingPlayers())
  {
    std::pair<double, std::string> best_command_for_player = getBestCommandForPlayer(player);
    best_commands[best_command_for_player.first] = best_command_for_player.second;
  }
  return (*best_commands.rbegin()).second;
}

std::pair<double, std::string> Decision::getBestCommandForPlayer(std::shared_ptr<Player> player)
{
  std::map<double, std::string> best_commands;
  if (game_->getDungeon().getCurrentRoom()->isComplete())
  {
    
  }
  else
  {

  }
}