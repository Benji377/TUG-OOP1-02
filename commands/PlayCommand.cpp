#include "PlayCommand.hpp"
#include "../game/Game.hpp"
#include "../robot/Robot.hpp"

void PlayCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);

  std::vector<std::shared_ptr<Player>> sharedPlayers = game_->getPlayers();
  std::vector<Player> players;
  std::transform(sharedPlayers.begin(), sharedPlayers.end(), std::back_inserter(players),
      [](const std::shared_ptr<Player>& ptr) {
          return *ptr;
      });
  
  updateState(game_->getState());
  game_->getRobot()->setCurrentState(*(game_->getState()));
  //game_->getRobot()->executeAction(RobotAction::MOVE_UP, players.at(0), players);
  //game_->getRobot()->executeAction(RobotAction::LOOT, players.at(0), players);
  //game_->getRobot()->executeAction(RobotAction::SET_RES_ACID, players.at(0), players);
  //game_->getRobot()->executeAction(RobotAction::ATTACK, players.at(0), players);
  //game_->getRobot()->executeAction(RobotAction::USE_RANGED, players.at(0), players);
  //game_->getRobot()->executeAction(RobotAction::USE_MELEE, players.at(0), players);
  //game_->getRobot()->executeAction(RobotAction::USE_ARMOR, players.at(0), players);
  //game_->getRobot()->executeAction(RobotAction::SWITCH_PLAYER, players.at(0), players);

  RobotAction action;
  double reward;
  do
  {
  action = game_->getRobot()->getBestAction(*(game_->getState()), players.at(0));
  reward = game_->getRobot()->executeAction(action, players.at(0), players);
  setCurrentRobotAction(action);
  setCurrentReward(reward);
  } while (action == RobotAction::SWITCH_PLAYER); //if it's switchplayer the next 4 lines will be handled automatically.
  
  // TODO: We need to update the state here, because the Q-Learning algorithm will update the Q-table based on the new state
  // and the previous state. We need to make sure that the state is updated before the Q-Learning algorithm is called.
  game_->getRobot()->setPreviousState(*(game_->getState()));
  updateState(game_->getState());
  game_->getRobot()->setCurrentState(*(game_->getState()));
  game_->getRobot()->updateQTable(action, players.at(0), reward);

}