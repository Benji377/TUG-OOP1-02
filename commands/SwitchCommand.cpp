#include "SwitchCommand.hpp"
#include "../game/Game.hpp"
#include "../robot/Robot.hpp"

void SwitchCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 2);

  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);

  game_->setActivePlayerQLearn(player->getAbbreviation());

  game_->getRobot()->setPreviousState(*(game_->getState()));
  updateState(game_->getState());
  game_->getRobot()->setCurrentState(*(game_->getState()));
  game_->getRobot()->updateQTable(getCurrentRobotAction(), *(game_->getPlayers().at(0)), getCurrentReward());
}