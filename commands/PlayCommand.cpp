#include "PlayCommand.hpp"
#include "../robot/State.hpp"

void PlayCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);
  State state = State();

  //game_->startAIStuff

}