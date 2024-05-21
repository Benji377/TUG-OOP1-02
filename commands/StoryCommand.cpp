#include "StoryCommand.hpp"
#include "../game/Game.hpp"

void StoryCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);
  game_->toggleStoryOutput();
}