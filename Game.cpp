#include "Game.hpp"




//---------------------------------------------------------------------------------------------------------------------
///
/// Parses the config files by first
///
/// @param dungeon_config_file Path to Dungeon Config File as String
/// @param story_config_file Path to Story Config File as String
///
/// @return Index of the left child node.
//
void Game::parseConfigs(char* dungeon_config_file, char* story_config_file)
{
  if (!IO::checkMagicNumber(dungeon_config_file, "DUNGEON"))
  {
    throw std::runtime_error("Invalid dungeon config file");
  }

  if (!IO::checkMagicNumber(story_config_file, "STORY"))
  {
    throw std::runtime_error("Invalid story config file");
  }

  //parse

}




