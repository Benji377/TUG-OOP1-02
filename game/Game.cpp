#include "Game.hpp"

#define MIN_PLAYERS 1
#define MAX_PLAYERS 3
#define MAX_NAME_LENGTH 10

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
  int num_players;
  while (true)
  {
    string input = IO::promtUserInput();

    if(input == "quit")
    {
      doCommand(input);
      break;
    }

    if (!Utils::decimalStringToInt(input, num_players) || num_players < MIN_PLAYERS || num_players > MAX_PLAYERS)
    {
      std::cout << "Please enter a number of players between " << MIN_PLAYERS << " and " << MAX_PLAYERS << "."
        << std::endl;
    }
    else
    {
      break;
    }
    Game::max_players_ = num_players;
  }
  for (int i = 1; i <= num_players; i++)
  {
    std::cout << "\nPlayer " << i << " what do you wish to be called? (max length " << MAX_NAME_LENGTH
      << " characters)" << std::endl;
    string name;
    while (true)
    {
      name.clear();
      name = IO::promtUserInput();

      if (name.length() > MAX_NAME_LENGTH || playerExists(name))
      {
        std::cout << "Please enter a unique name with not more than " << MAX_NAME_LENGTH << " characters." << std::endl;
      }
      else
      {
        break;
      }
    }
    std::cout << name << ", please choose a player type:" << std::endl;
    std::cout << "  [W] Wizard     " << getPlayerTypeAmount('W') << "/1" << std::endl;
    std::cout << "  [B] Barbarian  " << getPlayerTypeAmount('B') << "/1" << std::endl;
    std::cout << "  [R] Rogue      " << getPlayerTypeAmount('R') << "/1" << std::endl;
    char type;
    while (true)
    {
      string input;
      input = IO::promtUserInput();

      if(input == "quit")
      {
        doCommand(input);
      }

      Utils::normalizeString(input, true);
      if (input.length() != 1 || (input[0] != 'W' && input[0] != 'B' && input[0] != 'R'))
      {
        std::cout << "Please enter a letter representing your desired player type (W, B, or R)." << std::endl;
      } else if (getPlayerTypeAmount(input[0]) >= 1)
      {
        std::cout << "This player type is no longer available. Please choose a different player type." << std::endl;
      } else
      {
        type = input[0];
        break;
      }
    }
    std::shared_ptr<Player> player = std::make_shared<Player>(i, type, name);
    players_.push_back(player);
  }
  std::cout << "\n-- Players --------------------------------------" << std::endl;
  for (auto player : players_)
  {
    std::cout << "  ";
    player->simplePrintPlayer();
  }
  std::cout << std::endl;
}

void Game::doCommand()
{

  //Print Story Message
  std::string input = IO::promtUserInput();

  std::vector<std::string> command_input = IO::commandifyString(input);

  try
  {
    parser_->execute(command_input);
  }
  catch(const std::exception& e)
  {
    std::cout << e.what() << '\n';
  }

}

void Game::doCommand(std::string input)
{

  std::vector<std::string> command_input = IO::commandifyString(input);

  try
  {
    parser_->execute(command_input);
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

bool Game::playerExists(string name)
{
  for (auto player : players_)
  {
    if (player->getName() == name)
    {
      return true;
    }
  }
  return false;
}

int Game::getPlayerTypeAmount(char type)
{
  int count = 0;
  for (auto player : players_)
  {
    if (player->getAbbreviation() == type)
    {
      count++;
    }
  }
  return count;
}

std::shared_ptr<Room> Game::getCurrentRoom()
{
  return dungeon_.getCurrentRoom();
}