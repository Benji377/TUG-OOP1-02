#include "UseCommand.hpp"
#include "../game/Game.hpp"
#include "../entity/TreasureChest.hpp"
#include "../entity/DeathLocation.hpp"

void UseCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 3);

  std::string abbrev = params.at(2);
  std::transform(abbrev.begin(), abbrev.end(), abbrev.begin(), ::toupper);


  if(abbrev == "BOLT" || abbrev == "ARRW" || !(Utils::isValidItemAbbrev(abbrev)))
  {
    throw InvalidParamCommand();
  }

  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);
  std::shared_ptr<Inventory> player_inv = player->getInventory();

  std::shared_ptr<Potion> potion = player_inv->getPotion(abbrev);
  if(potion != nullptr)
  {
    player->simplePrintNoId();
    std::cout << " consumed \"" << potion->getName() << "\"." << std::endl;
    player->usePotion(abbrev);
    game_->plusOneActionCount();
    game_->printStoryAndRoom(false, true, true);
    return;
  }

  std::shared_ptr<Weapon> weapon = player_inv->getWeapon(abbrev);
  if(weapon != nullptr)
  {
    if(player->getWeapon() == weapon)
    {
      player->setActiveWeapon("");
    }
    else
    {
    player->setActiveWeapon(weapon->getAbbreviation());
    }

    game_->plusOneActionCount();
    game_->printStoryAndRoom(false, true, true);

    return;
  }

  shared_ptr<Armor> armor = player_inv->getArmor(abbrev);
  if(armor != nullptr)
  {
    if(player->getArmor() == armor)
    {
      player->setArmor("");
    }
    else
    {
      player->setArmor(armor->getAbbreviation());
    }
    game_->plusOneActionCount();
    game_->printStoryAndRoom(false, true, true);
    return;
  }

  //TODO figure out if an error should be seen as error and promt user again?

  throw UnavailableItemOrEntityCommand();

}