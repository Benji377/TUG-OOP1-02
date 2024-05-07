#include "Game.hpp"

#define MIN_PLAYERS 1
#define MAX_PLAYERS 3

Game::Game(char *dungeon_path, char *config_path) : dungeon_(Dungeon(dungeon_path))
{
  parser_ = std::make_unique<CommandParser>();
  story_.parseStory(config_path);
  is_running_ = true;
  story_output_active_ = true;
  parser_->registerCommand("help", std::make_unique<HelpCommand>());
  parser_->registerCommand("map", std::make_unique<MapCommand>(this));
  parser_->registerCommand("quit", std::make_unique<QuitCommand>(this));
  parser_->registerCommand("positions", std::make_unique<PositionsCommand>(this));
}

void Game::start()
{
  std::cout << Game::story_.getStorySegment("N_INTRO");
  std::cout << "How many players want to join the adventure? (" << MIN_PLAYERS << " to " << MAX_PLAYERS << ")"
            << std::endl;
  std::cout << "> ";
  int num_players;
  while (true)
  {
    string input;
    std::cin >> input;
    // TODO: Check if the input is quit or EOF
    if (!Utils::decimalStringToInt(input, num_players) || num_players < MIN_PLAYERS || num_players > MAX_PLAYERS)
    {
      std::cout << "Please enter a number of players between " << MIN_PLAYERS << " and " << MAX_PLAYERS << "."
                << std::endl;
      std::cout << "> ";
    }
    else
    {
      break;
    }
    Game::max_players_ = num_players;
  }

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