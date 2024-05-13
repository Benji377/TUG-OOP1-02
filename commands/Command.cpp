#include "Command.hpp"
#include "../game/Game.hpp"


void Command::checkParameterCount(std::vector<std::string> params, size_t required_size) const
{
  if(params.size() != required_size)
  {
    throw WrongNumberOfParametersException();
  }
}

void Command::isValidAbbrev(Abbrev type_of_abbrev, std::string input) const
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

}

std::shared_ptr<Player> Command::getPlayerOfAbbrev(std::vector<std::string> params, size_t position_of_abbrev_in_params) const
{
  std::string input_abbreviation = params.at(position_of_abbrev_in_params);

  isValidAbbrev(Abbrev::PLAYER, input_abbreviation);

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
  checkParameterCount(params, 1);

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
  checkParameterCount(params, 1);
  std::cout << "toggled MAP ouput" << std::endl;

  //game_->toggleMapOutPut();

}

void StoryCommand::execute(std::vector<std::string> params)
{
  checkParameterCount(params, 1);

  std::cout << "toggled STORY ouput" << std::endl;

  game_->toggleStoryOutput();

}



void QuitCommand::execute(std::vector<std::string> params)
{
  checkParameterCount(params, 1);

  game_->toggleGameRunning();

}

void PositionsCommand::execute(std::vector<std::string> params)
{
  checkParameterCount(params, 1);

  std::shared_ptr<Room> current_room = game_->getCurrentRoom();

  //Assuming Players are always stored by id, otherwise sort them
  std::vector<std::shared_ptr<Player>> players = game_->getPlayers();

  for(auto& player : players)
  {
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
  checkParameterCount(params, 2);

  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);
  player->printPlayer(game_->getCurrentRoom()->getFieldOfEntity(player), false);

}

void InventoryCommand::execute(std::vector<std::string> params)
{
  checkParameterCount(params, 2);

  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);

  IO::printInventory(player);

}