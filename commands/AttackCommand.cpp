#include "AttackCommand.hpp"
#include "../game/Game.hpp"
#include "../entity/TreasureChest.hpp"
#include "../entity/DeathLocation.hpp"

void AttackCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 3);

  shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);
  std::pair<int, int> target_position = getPositionAsPairOutOfString(params.at(2));

  if(player == nullptr)
  {
    throw UnavailableItemOrEntityCommand();
  }

  shared_ptr<Weapon> players_weapon = player->getWeapon();

  if(players_weapon == nullptr)
  {
    throw CommandExecutionException(CommandExecutionException::ExceptionType::NO_WEAPON_EQUIPPED);
    //TODO InvalidPos Command has higher priority that noweaponequipped. This cannot be checked in that order however
    //as the type of weapon influences the valid position
  }
  std::pair<int,int> current_position = game_->getCurrentRoom()->getFieldOfEntity(player);

  if(players_weapon->getAttackType() == AttackType::MELEE)
  {
    if(!(game_->getCurrentRoom()->isAdjacentField(target_position, current_position)) 
                      || target_position == current_position)
    {
      throw InvalidPositionCommand();
    }
  }
  else
  {
    if(!(game_->getCurrentRoom()->isValidField(target_position)))
    {
      throw InvalidPositionCommand();
    }

    //these 6 lines of code are from Benji, very beautiful!
    std::string ammoType = (players_weapon->getAbbreviation() == "SBOW" ||
                            players_weapon->getAbbreviation() == "LBOW") ? "ARRW" : "BOLT";

      if(player->getWeapon()->getAbbreviation().compare(0, 1, "Q") != 0)
      {
        if(player->getInventory()->getAmmunition(ammoType) == nullptr ||
            player->getInventory()->getAmmunition(ammoType)->getAmount() == 0)
        {
          throw CommandExecutionException(CommandExecutionException::ExceptionType::NO_AMMUNITION);
        }
      }
  }

  int damage = player->getAttackDamage();

  std::vector<AttackedField> attacked_fields_sorted =
                                  game_->getDungeon().characterAttack(player, damage, target_position);

  InputOutput::printSuccessFullAttack(player, target_position, attacked_fields_sorted);
  game_->plusOneActionCount();

  InputOutput::printDiceRoll(player->getWeapon()->getDice()->getPreviousRoll(), player->getWeapon()->getDice());

  InputOutput::printAttackedCharacters(attacked_fields_sorted);

  //Soly for a3 qlearning so it switches if a player kills another
  auto players = game_->getPlayers();
  for(auto field : attacked_fields_sorted)
  {
    if(field.getName() == players.at(0)->getName() || field.getName() == players.at(1)->getName()
        || field.getName() == players.at(2)->getName())
    {
      game_->setAdditionalreward(-10);
      if(game_->getActivePlayerQLearn()->isDead())
      {
          for(auto player : game_->getPlayers())
          {
            if(player->isDead() == false)
            {
              game_->setActivePlayerQLearn(player->getAbbreviation());
              break;
            }
          }
      }

    }
  }
  //Soly for a3 qlearning so it switches if a player kills another

  if(game_->getDungeon().isBossDead())
  {
    return;
  }

  if(game_->allPlayersAreDead())
  {
    return;
  }

  game_->printStoryAndRoom(false, true, true);

}