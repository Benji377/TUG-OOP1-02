#include "Command.hpp"
#include "../game/Game.hpp"
#include "../entity/Door.hpp"
#include "../dungeon/Field.hpp"
#include "../entity/TreasureChest.hpp"
#include "../entity/DeathLocation.hpp"
#include "../utility/Dice.hpp"

void Command::checkCommandLenght(std::vector<std::string> params, size_t required_size) const
{
  if(params.size() != required_size)
  {
    throw WrongNumberOfParametersException();
  }
}

/*void Command::isValidAbbrev(Abbrev type_of_abbrev, std::string input) const
{
  switch(type_of_abbrev)
  {
    case Abbrev::PLAYER:
       if(input == "b" || input == "r" || input == "w")
      {
        break;
      }
      else
      {
        throw InvalidParamCommand();
      }
      break;
  case Abbrev::ITEM:
      break;
  }

}*/

std::shared_ptr<Player> Command::getPlayerOfAbbrev(std::vector<std::string> params, size_t position_of_abbrev_in_params) const
{
  std::string input_abbreviation = params.at(position_of_abbrev_in_params);

  //isValidAbbrev(Abbrev::PLAYER, input_abbreviation);

  std::vector<std::shared_ptr<Player>> players = game_->getPlayers();
  char uppercase_input_abbrev = static_cast<char>(toupper(input_abbreviation[0]));
  for(auto& player : players)
  {
    char current_player_abbrev = player->getAbbreviation();

    if(current_player_abbrev == uppercase_input_abbrev)
    {
      return player;
    }
  }

  throw UnavailableItemOrEntityCommand();
}

void HelpCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);

  std::cout << "-- Commands ----------------------------------------\n"
                 "- help\n"
                 "     Prints this help text.\n"
                 "\n"
                 "- quit\n"
                 "    Terminates the game.\n"
                 "\n"
                 "- story\n"
                 "    Activates or deactivates the stories of the rooms.\n"
                 "\n"
                 "- map\n"
                 "    Activates or deactivates the map.\n"
                 "\n"
                 "- positions\n"
                 "    Prints the current positions of the players and enemies.\n"
                 "\n"
                 "- player <PLAYER_TYPE_ABBREVIATION>\n"
                 "    Prints player information of the specific player.\n"
                 "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
                 "\n"
                 "- inventory <PLAYER_TYPE_ABBREVIATION>\n"
                 "    Prints the inventory of the specific player.\n"
                 "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
                 "\n"
                 "* move <PLAYER_TYPE_ABBREVIATION> <POSITION>\n"
                 "    Lets the player move to an adjacent field.\n"
                 "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
                 "    <POSITION>: field at position: <ROW>,<COLUMN>\n"
                 "\n"
                 "* loot <PLAYER_TYPE_ABBREVIATION> <POSITION>\n"
                 "    Lets the player loot an adjacent chest.\n"
                 "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
                 "    <POSITION>: field at position: <ROW>,<COLUMN>\n"
                 "\n"
                 "* use <PLAYER_TYPE_ABBREVIATION> <ITEM_ABBREVIATION>\n"
                 "    Lets the player use a potion or equip armor or weapons.\n"
                 "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
                 "    <ITEM_ABBREVIATION>: abbreviation of an item in the players inventory\n"
                 "\n"
                 "* attack <PLAYER_TYPE_ABBREVIATION> <POSITION>\n"
                 "    Lets the player attack with the equipped weapon on the given position.\n"
                 "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
                 "    <POSITION>: field at position: <ROW>,<COLUMN>\n"
                 "\n"
                 "-- Legend ------------------------------------------\n"
                 "- display commands\n"
                 "* action commands (count as actions)\n"
                 "----------------------------------------------------\n";

}

void MapCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);

  game_->toggleMapOutput();

}

void StoryCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);

  game_->toggleStoryOutput();
}

void QuitCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);

  game_->toggleGameRunning();

}

void PositionsCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);

  std::shared_ptr<Room> current_room = game_->getCurrentRoom();

  //Assuming Players are always stored by id, otherwise sort them
  std::vector<std::shared_ptr<Player>> players = game_->getPlayers();

  for(auto& player : players)
  {
    // TODO: BENJI -> Check if this line is correct, may need to change the boolean value
    player->printPlayer(current_room->getFieldOfEntity(player), true);
  }

  std::vector<std::shared_ptr<Character>> enemies = current_room->getEnemies();

  std::map<std::string, std::shared_ptr<Character>> enemies_mapped;

  for(auto& enemy : enemies)
  {
    std::string enemy_string_to_be_sorted = enemy->getTypeName() + " " + std::to_string(enemy->getId());
    enemies_mapped.insert(std::make_pair(enemy_string_to_be_sorted, enemy));
  }

  IO::printEnemyPosition(enemies_mapped, current_room);
}


void PlayerCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 2);

  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);
  player->printPlayer(game_->getCurrentRoom()->getFieldOfEntity(player), false);

}

void InventoryCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 2);

  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);

  IO::printPlayerInventory(player);

}

std::vector<int> Command::getPositionAsVecOutOfString(std::string position_string)
{

  std::vector<int> position; //Works with a vector to check for stuff like 1,2,3

  try
  {
    //Code on how to exctract ints from a string from ChatGPT
    //begin
    std::istringstream iss(position_string);

    std::string token;

    while (std::getline(iss, token, ','))
    {
      int num = std::stoi(token);
      position.push_back(num);
    }
    //end
  }
  catch(const std::exception& e)
  {
    throw InvalidPositionCommand(); //If someone enters something very weird for the position like asd,asdasd
  }

  if(position.size() != 2 )
  {
    throw InvalidPositionCommand();
  }

  return position;
}

std::pair<int,int> Command::getPositionAsPairOutOfString(std::string position)
{
  std::vector<int> position_as_vector = getPositionAsVecOutOfString(position);
  std::pair<int, int> position_pair = std::make_pair(position_as_vector.at(0), position_as_vector.at(1));

  return position_pair;
}

std::pair<std::shared_ptr<Player>, std::pair<int, int>> Command::getPlayerAndAdjacentField(std::vector<std::string> params)
{
  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1); //Get player first to envoke right order of exceptions

  std::pair<int, int> target_position = getPositionAsPairOutOfString(params.at(2));

  std::pair<int,int> current_position = game_->getCurrentRoom()->getFieldOfEntity(player);

  //if it's not an adjacent field
  if(!(game_->getCurrentRoom()->isAdjacentField(current_position, target_position)))
  {
    throw InvalidPositionCommand();
  }

  return std::make_pair(player, target_position);
}

void MoveCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 3);

  std::pair<std::shared_ptr<Player>, std::pair<int,int>> player_and_field = getPlayerAndAdjacentField(params);
  std::shared_ptr<Player> player = player_and_field.first;
  std::pair<int,int> target_position = player_and_field.second;

  std::shared_ptr<Field> field = game_->getCurrentRoom()->getField(target_position);
  std::shared_ptr<Entity> entity_on_field = field->getEntity();

  if(entity_on_field == nullptr)
  {
    IO::printPlayerMoved(player, target_position);
    game_->getDungeon().characterMove(player, target_position);
    game_->printStoryAndRoom(false);
    game_->plusOneActionCount();
  }
  else if(std::dynamic_pointer_cast<Door>(entity_on_field) != nullptr)
  {
    std::shared_ptr<Door> door = std::dynamic_pointer_cast<Door>(entity_on_field);
    if(door->isLocked())
    {
      throw CommandExecutionException(CommandExecutionException::ExceptionType::LOCKED_DOOR);
    }
    else
    {
      IO::printPlayerMoved(player, target_position);
      int result = game_->getDungeon().moveToRoom(door->getLeadsTo());
      if (result == 1)
      {
        std::cout << game_->getStory().getStorySegment("N_FLIGHT");
        game_->toggleGameRunning();
        game_->printAndSaveScore();
        return;
      }
      game_->printStoryAndRoom();
    }

  }
  else
  {
    throw InvalidPositionCommand();
  }

}


void LootCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 3);

  std::pair<std::shared_ptr<Player>, std::pair<int,int>> player_and_field = getPlayerAndAdjacentField(params);
  std::shared_ptr<Player> player = player_and_field.first;
  std::pair<int,int> target_position = player_and_field.second;

  std::shared_ptr<Field> field = game_->getCurrentRoom()->getField(target_position);
  std::shared_ptr<Entity> entity_on_field = field->getEntity();

  if(std::dynamic_pointer_cast<DeathLocation>(entity_on_field) != nullptr)
  {
    game_->getDungeon().lootEntity(player, entity_on_field);
    game_->plusOneActionCount();
  }
  else if(std::dynamic_pointer_cast<TreasureChest>(entity_on_field) != nullptr)
  {
    std::shared_ptr<TreasureChest> chest = std::dynamic_pointer_cast<TreasureChest>(entity_on_field);

    int min_value_to_roll = chest->getMinValue() - player->getVitality();

    std::cout << "** To unlock this chest you need to roll at least "
    << min_value_to_roll << " to reach the " << chest->getMinValue() << " needed.\n";

    std::shared_ptr<Dice> dice = std::make_shared<Dice>("1 d20");
    int roll_result = dice->roll();
    game_->plusOneActionCount();
    IO::printDiceRoll(roll_result, dice);

    if(roll_result < min_value_to_roll)
    {
      std::cout << game_->getStory().getStorySegment("N_LOOT_CHEST_LOCKED");
    }
    else
    {
      std::cout << game_->getStory().getStorySegment("N_LOOT_SUCCESSFUL");

      std::map<string, int> loot = entity_on_field->getLoot();
      std::shared_ptr<Inventory> inv_of_entity = std::make_shared<Inventory>();
      int ret = inv_of_entity->parseInventory(loot);
      if (ret == 1)
      {
        std::cout << "The entity contains an unknown item. The loot could not be parsed." << std::endl;
      }
      IO::printInventory(inv_of_entity, nullptr);
      game_->getDungeon().lootEntity(player, entity_on_field);
      game_->plusOneActionCount();
    }
  }
  else
  {
    throw InvalidPositionCommand();
  }

}


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
    std::cout << " consumed " << potion->getName() << std::endl;
    player->usePotion(abbrev);
    game_->plusOneActionCount();
    return;
  }

  std::shared_ptr<Weapon> weapon = player_inv->getWeapon(abbrev);
  if(weapon != nullptr)
  {
    if(player->getActiveWeapon() == weapon)
    {
      player->setActiveWeapon(nullptr);
    }
    else
    {
    player->setActiveWeapon(weapon->getAbbreviation());
    }
    game_->plusOneActionCount();
    return;
  }

  shared_ptr<Armor> armor = player_inv->getArmor(abbrev);
  if(armor != nullptr)
  {
    if(player->getArmor() == armor)
    {
      player->setArmor(nullptr);
    }
    else
    {
      player->setArmor(abbrev);
    }
    game_->plusOneActionCount();
    return;
  }

  //TODO figure out if an error should be seen as error and promt user again?

  throw UnavailableItemOrEntityCommand();

}

void AttackCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 3);

  shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);
  shared_ptr<Weapon> players_weapon = player->getActiveWeapon();

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
    if(!(game_->getCurrentRoom()->isAdjacentField(target_position, current_position)))
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
    if (player->getInventory()->getAmmunition(ammoType) == nullptr ||
                            player->getInventory()->getAmmunition(ammoType)->getAmount() == 0)
    {
      throw CommandExecutionException(CommandExecutionException::ExceptionType::NO_AMMUNITION);
    }

  }

  int damage = player->getAttackDamage(); //TODO is this supposed to throw an exception?

  std::vector<AttackedField> attacked_fields_sorted = game_->getDungeon().characterAttack(player, damage, target_position);

  IO::printSuccessFullAttack(player, target_position, attacked_fields_sorted);
  // TODO: Benji -> Since the damage no longer corresponds to the dice roll, I created a function that provides the last dice roll
  IO::printDiceRoll(player->getActiveWeapon()->getDice()->getPreviousRoll(), player->getActiveWeapon()->getDice());

  IO::printAttackedCharacters(attacked_fields_sorted);

  game_->printStoryAndRoom(false);

  game_->plusOneActionCount();
}


