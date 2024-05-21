#include "AttackCommand.hpp"
#include "../game/Game.hpp"
#include "../entity/TreasureChest.hpp"
#include "../entity/DeathLocation.hpp"

void AttackCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 3);

  shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);
  shared_ptr<Weapon> players_weapon = player->getWeapon();

  if(players_weapon == nullptr)
  {
    throw CommandExecutionException(CommandExecutionException::ExceptionType::NO_WEAPON_EQUIPPED);
    //TODO InvalidPos Command has higher priority that noweaponequipped. This cannot be checked in that order however
    //as the type of weapon influences the valid position
  }

  std::pair<int, int> target_position = getPositionAsPairOutOfString(params.at(2));
  std::pair<int,int> current_position = game_->getCurrentRoom()->getFieldOfEntity(player);

  if(players_weapon->getAttackType() == AttackType::MELEE)
  {
    if(!(game_->getCurrentRoom()->isAdjacentField(target_position, current_position)) || target_position == current_position)
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

  std::vector<AttackedField> attacked_fields_sorted = game_->getDungeon().characterAttack(player, damage, target_position);

  IO::printSuccessFullAttack(player, target_position, attacked_fields_sorted);
  game_->plusOneActionCount();

  IO::printDiceRoll(player->getWeapon()->getDice()->getPreviousRoll(), player->getWeapon()->getDice());

  IO::printAttackedCharacters(attacked_fields_sorted);

  if(game_->getDungeon().isBossDead())
  {
    return;
  }

  game_->printStoryAndRoom(false, true, true);

}