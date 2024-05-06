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

Game::Game()
{
  parser_ = std::make_unique<CommandParser>();
  is_running_ = false;
}


//Idea to use map for commands from KU Practicals 5
void Game::start()
{
  is_running_ = true;

  parser_->registerCommand("help", std::make_unique<HelpCommand>());
  parser_->registerCommand("quit", std::make_unique<QuitCommand>(this));
}

void Game::doCommand(std::string input)
{
  parser_->execute(input);
}

void Game::toggleGame()
{
  is_running_ = !is_running_;
}

//If game isn't declared here, I get a seg fault
Game::~Game()
{}

bool Game::isRunning() const
{
  return is_running_;
}