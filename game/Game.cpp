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
    std::string error_message = "Invalid config file ";
    error_message += dungeon_config_file;
    throw std::runtime_error(error_message);
  }

  if (!IO::checkMagicNumber(story_config_file, "STORY"))
  {
    std::string error_message = "Invalid config file ";
    error_message += story_config_file;
    throw std::runtime_error(error_message);
  }

  //parse

}

//Idea to use map for commands from KU Practicals 5
void Game::start()
{
  parser_ = new CommandParser;

  parser_->registerCommand("help", new HelpCommand());
  parser_->registerCommand("quit", new QuitCommand(this));

  std::string input = "quit";
  parser_->execute(input);
}

void Game::toggleGame()
{
  is_running_ = !is_running_;
}

Game::~Game()
{
  delete parser_;
}

