#include "Game.hpp"
#include <algorithm>

#define M_PI 3.14159265358979323846
#define MIN_PLAYERS 1
#define MAX_PLAYERS 3
#define MAX_NAME_LENGTH 10

Game::Game(char *dungeon_path, char *config_path) : dungeon_(Dungeon(dungeon_path))
{
  parser_ = std::make_unique<CommandParser>();
  story_.parseStory(config_path);
  current_phase_ = Phase::ACTION;
  parser_->registerCommand("help", std::make_unique<HelpCommand>());
  parser_->registerCommand("map", std::make_unique<MapCommand>(this));
  parser_->registerCommand("story", std::make_unique<StoryCommand>(this));
  parser_->registerCommand("quit", std::make_unique<QuitCommand>(this));
  parser_->registerCommand("positions", std::make_unique<PositionsCommand>(this));
  parser_->registerCommand("player", std::make_unique<PlayerCommand>(this));
  parser_->registerCommand("inventory", std::make_unique<InventoryCommand>(this));

  parser_->registerCommand("move", std::make_unique<MoveCommand>(this));
  parser_->registerCommand("loot", std::make_unique<LootCommand>(this));
  parser_->registerCommand("use", std::make_unique<UseCommand>(this));
  parser_->registerCommand("attack", std::make_unique<AttackCommand>(this));


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
  }
  Game::max_players_ = num_players;
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

void Game::step()
{
  if (current_phase_ == Phase::ACTION)
  {
    if (action_count_ < max_players_)
    {
      doCommand();
    }
    else
    {
      current_phase_ = Phase::ENEMY;
      action_count_ = 0;
    }
  }
  else if (current_phase_ == Phase::ENEMY)
  {
    enemyPhase();
    printStoryAndRoom(false);
    current_phase_ = Phase::ACTION;
  }
  dungeon_.getCurrentRoom()->checkCompletion();
  dungeon_.getCurrentRoom()->openDoors();
  if (allPlayersAreDead())
  {
    std::cout << Game::story_.getStorySegment("N_DEFEAT");
    is_running_ = false;
    printAndSaveScore();
    return;
  }
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
      case CommandExecutionException::ExceptionType::NO_WEAPON_EQUIPPED:
        std::cout << Game::story_.getStorySegment("E_ATTACK_NO_WEAPON_EQUIPPED");
        break;
      case CommandExecutionException::ExceptionType::NO_AMMUNITION:
        std::cout << Game::story_.getStorySegment("E_ATTACK_NO_AMMUNITION");
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
  }
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


bool Game::allPlayersAreDead()
{
  for (auto player : players_)
  {
    if (!player->isDead())
    {
      return false;
    }
  }
  return true;
}

void Game::plusOneActionCount()
{
  action_count_++;
  total_actions_++;
}

std::ostream& Game::returnScoreOutput(std::ostream& os)
{
  os << "-- Players --------------------------------------" << std::endl;
  for (auto player : players_)
  {
    if (!player->isDead())
    {
      os << "  " << player->getTypeName() << " [" << player->getAbbreviation() << "] \"" << player->getName()
        << "\" survived." << std::endl;
    }
  }
  for (auto player : players_)
  {
    if (player->isDead())
    {
      os << "  " << player->getTypeName() << " [" << player->getAbbreviation() << "] \"" << player->getName()
        << "\", rest in peace." << std::endl;
    }
  }
  os << "\n-- Statistics -----------------------------------" << std::endl;
  os << "  " << dungeon_.getCompletedRoomsCount() << " rooms completed" << std::endl;
  os << "  " << total_actions_ << " performed actions\n" << std::endl;
  return os;
}

void Game::printAndSaveScore()
{
  returnScoreOutput(std::cout);
  std::cout << Game::story_.getStorySegment("N_SCORING_FILE");
  while (true)
  {
    std::string file_name = IO::promtUserInput();
    if(file_name == "quit")
    {
      doCommand(file_name);
      return;
    }
    std::ofstream file(file_name, std::ios::out | std::ios::trunc);
    if (file.is_open())
    {
      returnScoreOutput(file);
      file.close();
      break;
    }
    else
    {
      std::cout << Game::story_.getStorySegment("E_SCORING_FILE_NOT_WRITABLE");
    }
  }
}

void Game::enemyPhase()
{
  std::vector<std::shared_ptr<Enemy>> enemies = dungeon_.getCurrentRoom()->getAllEntitiesOfType<Enemy>();
    std::sort(enemies.begin(), enemies.end(),
    [](const std::shared_ptr<Character>& a, const std::shared_ptr<Character>& b) -> bool
    {
      if (a->getAbbreviation() == b->getAbbreviation())
      {
        return a->getId() < b->getId();
      }
      return a->getAbbreviation() < b->getAbbreviation();
    });
  for (auto enemy : enemies)
  {
    std::vector<std::shared_ptr<Player>> players = dungeon_.getCurrentRoom()->getAllEntitiesOfType<Player>();
    std::sort(players.begin(), players.end(), [this, enemy](const std::shared_ptr<Player>& a, const std::shared_ptr<Player>& b) {
      std::pair<int, int> a_pos = dungeon_.getCurrentRoom()->getFieldOfEntity(a);
      std::pair<int, int> b_pos = dungeon_.getCurrentRoom()->getFieldOfEntity(b);
      std::pair<int, int> enemy_pos = dungeon_.getCurrentRoom()->getFieldOfEntity(enemy);

      int distA = std::abs(a_pos.first - enemy_pos.first) + std::abs(a_pos.second - enemy_pos.second);
      int distB = std::abs(b_pos.first - enemy_pos.first) + std::abs(b_pos.second - enemy_pos.second);

      if (distA == distB)
      {
        if (a->getHealth() == b->getHealth())
        {
          // Start code from copilot TODO: Does not work
          double angleA = atan2(a_pos.second - enemy_pos.second, a_pos.first - enemy_pos.first);
          double angleB = atan2(b_pos.second - enemy_pos.second, b_pos.first - enemy_pos.first);
          // End code from copilot
          return angleA < angleB;
        }
        return a->getHealth() < b->getHealth();
      }
      return distA < distB;
    });
    for (auto player : players)
    {
      std::pair<int, int> enemy_pos = dungeon_.getCurrentRoom()->getFieldOfEntity(enemy);
      std::pair<int, int> player_pos = dungeon_.getCurrentRoom()->getFieldOfEntity(player);
      if (dungeon_.getCurrentRoom()->isAdjacentField(enemy_pos, player_pos))
      {
        int damage = enemy->getAttackDamage();
        std::vector<AttackedField> attacked_fields = getDungeon().characterAttack(enemy, damage, player_pos);
        IO::printSuccessFullAttack(enemy, player_pos, attacked_fields);
        IO::printDiceRoll(damage, enemy->getWeapon()->getDice());
        IO::printAttackedCharacters(attacked_fields);
        break;
      }
      else
      {
        int damage = enemy->getAttackDamage(AttackType::RANGED);
        if (damage == -1)
        {
          dungeon_.moveToRandomField(enemy);
          std::cout << "\n" << enemy->getTypeName() << " " << enemy->getId() << " [" << enemy->getAbbreviation() <<
            enemy->getId() << "] moved to (" << enemy_pos.first << "," << enemy_pos.second << ")." << std::endl;
          break;
        }
        std::vector<AttackedField> attacked_fields = getDungeon().characterAttack(enemy, damage, player_pos);
        IO::printSuccessFullAttack(enemy, player_pos, attacked_fields);
        IO::printDiceRoll(damage, enemy->getWeapon()->getDice());
        IO::printAttackedCharacters(attacked_fields);
        break;
      }
    }
  }
}