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
  parser_->registerCommand("story", std::make_unique<StoryCommand>(this));
  parser_->registerCommand("quit", std::make_unique<QuitCommand>(this));
  parser_->registerCommand("positions", std::make_unique<PositionsCommand>(this));
  parser_->registerCommand("player", std::make_unique<PlayerCommand>(this));
  parser_->registerCommand("inventory", std::make_unique<InventoryCommand>(this));

}

void Game::start()
{
  std::cout << Game::story_.getStorySegment("N_INTRO");
  std::cout << "How many players want to join the adventure? (" << MIN_PLAYERS << " to " << MAX_PLAYERS << ")"
    << std::endl;
  int num_players;
  Game::max_players_ = 3;

  /*while (true)
  {
    string input = IO::promtUserInput();

    if(input == "quit")
    {
      doCommand(input);
      return;
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
    Game::max_players_ = 3;//num_players;
  }/*
  for (int i = 1; i <= num_players; i++)
  {
    std::cout << "\nPlayer " << i << " what do you wish to be called? (max length " << MAX_NAME_LENGTH
      << " characters)" << std::endl;
    string name;
    while (true)
    {
      name.clear();
      name = IO::promtUserInput(false);
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
      // TODO: If on the previous input EOF was entered, the program should go on
      string input = IO::promtUserInput();
      if(input == "quit")
      {
        doCommand(input);
        return;
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
    std::shared_ptr<Player> player = std::make_shared<Player>(i, type, name);*/

//You might get collision errors when running it with different configs than
//"dungeon_config.txt" because you might spawn the players into something else
//If you want to work with stuff above, uncomment and remove this:
//begin
    std::shared_ptr<Player> player1 = std::make_shared<Player>(1, 'W', "john");
    players_.push_back(player1);
    dungeon_.getCurrentRoom()->setFieldEntity(player1, 1, 1);

    std::shared_ptr<Player> player2 = std::make_shared<Player>(2, 'B', "jimmy");
    players_.push_back(player2);
    dungeon_.getCurrentRoom()->setFieldEntity(player2, 1, 3);

    std::shared_ptr<Player> player3 = std::make_shared<Player>(3, 'R', "jay");
    dungeon_.getCurrentRoom()->setFieldEntity(player3, 1, 4);
    players_.push_back(player3);
//end    
 
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

  std::cout << Game::story_.getStorySegment("N_PROMPT_MESSAGE");

  std::string input = IO::promtUserInput();

  std::vector<std::string> command_input = IO::commandifyString(input);

  try
  {
    parser_->execute(command_input);
  }
  catch(const UnknownCommand& e)
  {
    std::cout << Game::story_.getStorySegment("E_UNKNOWN_COMMAND");
  }
  catch(const WrongNumberOfParametersException& e)
  {
    std::cout << Game::story_.getStorySegment("E_INVALID_PARAM_COUNT");
  }
  catch(const InvalidParamCommand& e)
  {
    std::cout << Game::story_.getStorySegment("E_INVALID_PARAM");
  }
  catch(const UnavailableItemOrEntityCommand&e)
  {
    std::cout << Game::story_.getStorySegment("E_ENTITY_OR_ITEM_UNAVAILABLE");
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

void Game::toggleGameRunning()
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


