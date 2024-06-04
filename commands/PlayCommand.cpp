#include "PlayCommand.hpp"
#include "../game/Game.hpp"
#include "../decision/Decision.hpp"

void PlayCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);
  Decision decision(game_);
  std::string command = decision.getBestCommand();
  std::cout << "AI: " << command << std::endl;
}