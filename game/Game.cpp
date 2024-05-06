#include "Game.hpp"
Game::Game()
{
  parser_ = std::make_unique<CommandParser>();
  is_running_ = false;
  story_output_active_ = true;
}

Game::Game(char *dungeon_path, char *config_path)
{
  story = StorySegment::loadStory(config_path);
  parser_ = std::make_unique<CommandParser>();
  is_running_ = false;
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