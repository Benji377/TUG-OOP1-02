#include "SwitchCommand.hpp"
#include "../game/Game.hpp"

void SwitchCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 2);

  std::cout << "[DEBUG] SwitchCommand::execute, params:" << std::endl;
  for (auto &param : params)
  {
    std::cout << param << std::endl;
  }
  std::cout << "[DEBUG] SwitchCommand::execute, end of params" << std::endl;

  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);
  std::cout << "Switching to player " << player->getName() << std::endl;

  game_->setActivePlayerQLearn(player->getAbbreviation());

  game_->getRobot()->setPreviousState(*(game_->getState()));
  updateState(game_->getState());
  game_->getRobot()->setCurrentState(*(game_->getState()));
  game_->getRobot()->updateQTable(getCurrentRobotAction(), *(game_->getPlayers().at(0)), getCurrentReward());
  
  game_->setAdditionalreward(0);
}