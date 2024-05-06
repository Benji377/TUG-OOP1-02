#include "Game.hpp"
Game::Game()
{
  parser_ = std::make_unique<CommandParser>();
  is_running_ = false;
}

Game::Game(char *dungeon_path, char *config_path)
{
  story = StorySegment::loadStory(config_path);
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

bool Game::isRunning() const
{
  return is_running_;
}