#include "QuitCommand.hpp"
#include "../game/Game.hpp"

void QuitCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);

  game_->toggleGameRunning();

}