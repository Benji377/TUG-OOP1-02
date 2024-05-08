#include "Command.hpp"
#include "../game/Game.hpp"


void Command::checkParameterCount(std::vector<std::string> params, size_t required_size)
{
  if(params.size() != required_size)
  {
    throw std::invalid_argument("Invalid Parameter Count");
  }
}

void HelpCommand::execute(std::vector<std::string> params)
{
  checkParameterCount(params, 1);

  //TODO make pretty, put in IOor use csv file
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

  game_->toggleStoryOutput();

}

void QuitCommand::execute(std::vector<std::string> params)
{
  checkParameterCount(params, 1);

  std::cout << "quit" << std::endl;

  game_->toggleGameRunning();

}


void PositionsCommand::execute(std::vector<std::string> params)
{
  checkParameterCount(params, 1);

  std::shared_ptr<Room> current_room = game_->getCurrentRoom();

  //vector<vector<shared_ptr<Field>>> fields = current_room->getCharacters();

}