#include "MapCommand.hpp"
#include "../game/Game.hpp"

void MapCommand::execute(std::vector<std::string> params) 
{
  checkCommandLenght(params, 1);

  game_->toggleMapOutput();

  if(game_->getMapOutPutState() == true)
  {
    game_->printStoryAndRoom(false, false, false);
  }
}