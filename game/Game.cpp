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
  story_output_active_ = true;
}


void Game::start()
{
  is_running_ = true;

  parser_->registerCommand("help", std::make_unique<HelpCommand>());
  //parser_->registerCommand("map", std::make_unique<MapCommand>(this));
  parser_->registerCommand("quit", std::make_unique<QuitCommand>(this));
}

void Game::doCommand()
{

  //Print Story Message
  std::vector<std::string> input = IO::promtUserInput();

  try
  {
    parser_->execute(input);
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }

}

void Game::toggleStoryOutput()
{
  story_output_active_ = !story_output_active_;
}

void Game::toggleGame()
{
  is_running_ = !is_running_;
}

bool Game::isRunning() const
{
  return is_running_;
}