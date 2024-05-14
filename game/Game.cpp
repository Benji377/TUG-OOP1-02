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

  parser_->registerCommand("move", std::make_unique<MoveCommand>(this));
  parser_->registerCommand("loot", std::make_unique<MoveCommand>(this));


}

void Game::start()
{
  std::cout << Game::story_.getStorySegment("N_INTRO");
  std::cout << "How many players want to join the adventure? (" << MIN_PLAYERS << " to " << MAX_PLAYERS << ")"
    << std::endl;
  int num_players;
  Game::max_players_ = 3;

  while (true)
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
    std::shared_ptr<Player> player = std::make_shared<Player>(i, type, name);
    players_.push_back(player);
  }
 
  std::cout << "\n-- Players --------------------------------------" << std::endl;
  for (auto player : players_)
  {
    std::cout << "  ";
    player->simplePrint();
  }
  std::cout << std::endl;
  dungeon_.enterCurrentRoom(0, players_);
  printStoryAndRoom();
}


void Game::doCommand()
{
  command_finished_ = false;

  std::cout << Game::story_.getStorySegment("N_PROMPT_MESSAGE");

  while(!command_finished_)
  {
  std::string input = IO::promtUserInput();

  std::vector<std::string> command_input = IO::commandifyString(input);

  try
  {
    parser_->execute(command_input);
  }
  catch(const UnknownCommand& e)
  {
    std::cout << Game::story_.getStorySegment("E_UNKNOWN_COMMAND");
    continue;
  }
  catch(const WrongNumberOfParametersException& e)
  {
    std::cout << Game::story_.getStorySegment("E_INVALID_PARAM_COUNT");
    continue;
  }
  catch(const InvalidParamCommand& e)
  {
    std::cout << Game::story_.getStorySegment("E_INVALID_PARAM");
    continue;
  }
  catch(const UnavailableItemOrEntityCommand&e)
  {
    std::cout << Game::story_.getStorySegment("E_ENTITY_OR_ITEM_UNAVAILABLE");
    continue;
  }
  catch(const InvalidPositionCommand& e)
  {
    std::cout << Game::story_.getStorySegment("E_INVALID_POSITION");
    continue;
  }
  catch(const CommandExecutionException& e)
  {
    switch(e.getType())
    {
      case CommandExecutionException::ExceptionType::LOCKED_DOOR:
        std::cout << Game::story_.getStorySegment("E_MOVE_LOCKED_DOOR");
        break;
      default:
        break;
    }


  continue;
  }

  command_finished_ = true;
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

std::shared_ptr<Player> Game::getPlayerByType(char type)
{
  for (auto player : players_)
  {
    if (player->getAbbreviation() == type)
    {
      return player;
    }
  }
  return nullptr;
}

std::shared_ptr<Room> Game::getCurrentRoom()
{
  return dungeon_.getCurrentRoom();
}

void Game::printStoryAndRoom(bool print_story)
{
  if(!dungeon_.getCurrentRoom()->isComplete() && story_output_active_ && print_story)
  {
    std::cout << Game::story_.getStorySegment("N_ROOM_" + std::to_string(dungeon_.getCurrentRoom()->getId()));
    vector<char> new_enemies = Utils::getDifference(dungeon_.getCurrentRoom()->getEnemiesAbbreviations(),
              dungeon_.getOccuredEnemyTypes());
    Utils::deleteDuplicates(new_enemies);
    if(new_enemies.size() > 0)
    {
      for (auto enemy : new_enemies)
      {
        std::cout << Game::story_.getStorySegment("N_ENEMY_" + string(1, enemy));
        dungeon_.addOccuredEnemyType(enemy);
      }
    }
  }
  std::cout << "\n-- ROOM " << dungeon_.getCurrentRoom()->getId() << " (" << dungeon_.getCompletedRoomsCount()
    << "/" << dungeon_.getRoomCount() << " completed) --------------------\n" << std::endl;
  if (map_output_active_)
  {
    dungeon_.printCurrentRoom();
    std::vector<std::shared_ptr<Character>> enemies = dungeon_.getCurrentRoom()->getEnemies();
    if (enemies.size() > 0)
    {
      std::cout << "   ";
      for (auto enemy : enemies)
      {
        enemy->simplePrint();
        if (enemy != enemies.back())
        {
          std::cout << ", ";
        }
        else
        {
          std::cout << std::endl;
        }
      }
    }
  }
}

void Game::moveToRoom(int room_id)
{
  dungeon_.moveToRoom(room_id);
  printStoryAndRoom();
}

void Game::movePlayer(char player_abbrev, std::pair<int, int> position)
{
  std::shared_ptr<Player> player = getPlayerByType(player_abbrev);
  dungeon_.moveCharacter(player, position);
  std::cout << player->getTypeName() << " [" << player->getAbbreviation() << "] \"" << player->getName()
    << "\" moved to (" << position.first << ", " << position.second << ")." << std::endl;
  action_count_++;
  printStoryAndRoom(false);
}

void Game::lootEntity(std::shared_ptr<Player> player, std::shared_ptr<Entity> entity)
{
  /*int ret = player->getInventory()->parseInventory(entity->getLoot());
  if (ret == 1)
  {
    std::cout << "The entity contains an unknown item. The loot could not be parsed." << std::endl;
  }
  std::pair<int, int> position = dungeon_.getCurrentRoom()->getFieldOfEntity(entity);
  dungeon_.getCurrentRoom()->setFieldEntity(nullptr, position.first, position.second);*/
}