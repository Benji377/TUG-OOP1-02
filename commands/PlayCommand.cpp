#include "PlayCommand.hpp"
#include "../game/Game.hpp"
#include "../robot/Robot.hpp"

void PlayCommand::updateState(shared_ptr<State> state)
{
  //TODO decide on current player. Currently just takes the first player.
  std::shared_ptr<Player> current_player = game_->getPlayers().at(0);
  state->setCurrentPosition(game_->getCurrentRoom()->getFieldOfEntity(current_player));
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
  state->setEnemies(game_->getCurrentRoom()->getCharacterAsInt<Enemy>());
  state->setPlayers(game_->getCurrentRoom()->getCharacterAsInt<Player>());
  state->setLootables(game_->getCurrentRoom()->getLootableAsInt());
  //TODO implement setter for Doors, DeathLocations and Chests
}

void PlayCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);

  updateState(game_->getState());
  //game_->getRobot()->executeAction(game_->getRobot()->getBestAction());
}