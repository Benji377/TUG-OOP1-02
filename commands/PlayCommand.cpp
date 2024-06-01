#include "PlayCommand.hpp"
#include "../game/Game.hpp"
#include "../robot/Robot.hpp"

void PlayCommand::updateState(shared_ptr<State> state)
{
  auto current_room = game_->getCurrentRoom();
  //TODO decide on current player. Currently just takes the first player.
  std::shared_ptr<Player> current_player = game_->getPlayers().at(0);

  auto current_pos = current_room->getFieldOfEntity(current_player);
  std::pair<int, int> new_pos = std::make_pair(current_pos.first - 1, current_pos.second - 1);
  state->setCurrentPosition(new_pos);

  state->setHealth(current_player->getHealth());
  state->setRemainingActionCount(game_->getActionCount());
  std::shared_ptr<Weapon> weapon = current_player->getWeapon();
  state->setDamageOutput(weapon->getDamage());
  if(weapon->getAttackType() == AttackType::MELEE)
  {
    state->setCanAttackRange(false);
  }
  else
  {
    state->setCanAttackRange(true);
  }
  state->setCanAttackMelee(true);
  auto armor = current_player->getArmor();
  if(armor) state->setDamageInput(armor->getArmorValue());
  std::vector<std::shared_ptr<Potion>> potions = current_player->getInventory()->getAllPotions();
  for(auto potion : potions)
  {
    if(potion->getEffect() == Effect::HEALTH)
    {
      state->setCanHeal(true);
      break;
    }
  }
  state->setEnemies(current_room->getCharacterAsInt<Enemy>());
  state->setPlayers(current_room->getCharacterAsInt<Player>());
  state->setLootables(current_room->getLootableAsInt());
  state->setDoorPosition(current_room->getNextDoorPosition());
}

void PlayCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);

  std::vector<std::shared_ptr<Player>> sharedPlayers = game_->getPlayers();
  std::vector<Player> players;
  std::transform(sharedPlayers.begin(), sharedPlayers.end(), std::back_inserter(players),
      [](const std::shared_ptr<Player>& ptr) {
          return *ptr; // Dereference the shared_ptr to get the Player object
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

auto action = game_->getRobot()->getBestAction(*(game_->getState()), players.at(0));
game_->getRobot()->executeAction(action, players.at(0), players);

}