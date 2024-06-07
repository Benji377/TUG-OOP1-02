#include "Game.hpp"
#include <algorithm>

using std::cout;
using std::endl;
using std::make_unique;
using std::pair;
using std::string;

Game::Game(char *dungeon_path, char *config_path) : dungeon_(Dungeon(dungeon_path))
{
  parser_ = make_unique<CommandParser>();
  story_.parseStory(config_path);
  current_phase_ = Phase::ACTION;
  parser_->registerCommand("help", make_unique<HelpCommand>());
  parser_->registerCommand("map", make_unique<MapCommand>(this));
  parser_->registerCommand("story", make_unique<StoryCommand>(this));
  parser_->registerCommand("quit", make_unique<QuitCommand>(this));
  parser_->registerCommand("positions", make_unique<PositionsCommand>(this));
  parser_->registerCommand("player", make_unique<PlayerCommand>(this));
  parser_->registerCommand("inventory", make_unique<InventoryCommand>(this));

  parser_->registerCommand("move", make_unique<MoveCommand>(this));
  parser_->registerCommand("loot", make_unique<LootCommand>(this));
  parser_->registerCommand("use", make_unique<UseCommand>(this));
  parser_->registerCommand("attack", make_unique<AttackCommand>(this));

  parser_->registerCommand("play", make_unique<PlayCommand>(this));
  parser_->registerCommand("whoami", make_unique<WhoamiCommand>());
}

void Game::start()
{
  cout << Game::story_.getStorySegment("N_INTRO");
  cout << "How many players want to join the adventure? (" << MIN_PLAYERS << " to " << MAX_PLAYERS << ")" << endl;
  int num_players;
  while (true)
  {
    string input = InputOutput::promtUserInput();
    if (input == "quit")
    {
      doCommand(input);
      return;
    }
    if (!Utils::decimalStringToInt(input, num_players) || num_players < MIN_PLAYERS || num_players > MAX_PLAYERS)
    {
      cout << "Please enter a number of players between " << MIN_PLAYERS << " and " << MAX_PLAYERS << "." << endl;
    }
    else
    {
      break;
    }
  }
  Game::max_players_ = num_players;
  for (int player_index = 1; player_index <= num_players; player_index++)
  {
    cout << "\nPlayer " << player_index << " what do you wish to be called? (max length " << MAX_NAME_LENGTH
         << " characters)" << endl;
    string name;
    while (true)
    {
      name.clear();
      name = InputOutput::promtUserInput(false);
      if (name.length() > MAX_NAME_LENGTH || playerExists(name))
      {
        cout << "Please enter a unique name with not more than " << MAX_NAME_LENGTH << " characters." << endl;
      }
      else
      {
        break;
      }
    }
    cout << name << ", please choose a player type:" << endl;
    cout << "  [W] Wizard     " << getPlayerTypeAmount('W') << "/1" << endl;
    cout << "  [B] Barbarian  " << getPlayerTypeAmount('B') << "/1" << endl;
    cout << "  [R] Rogue      " << getPlayerTypeAmount('R') << "/1" << endl;
    char type;
    while (true)
    {
      string input = InputOutput::promtUserInput();
      if (input == "quit")
      {
        doCommand(input);
        return;
      }
      Utils::normalizeString(input, true);
      if (input.length() != 1 || (input[0] != 'W' && input[0] != 'B' && input[0] != 'R'))
      {
        cout << "Please enter a letter representing your desired player type (W, B, or R)." << endl;
      }
      else if (getPlayerTypeAmount(input[0]) >= 1)
      {
        cout << "This player type is no longer available. Please choose a different player type." << endl;
      }
      else
      {
        type = input[0];
        break;
      }
    }
    shared_ptr<Player> player = std::make_shared<Player>(player_index, type, name);
    players_.push_back(player);
  }
  cout << "\n-- Players --------------------------------------" << endl;
  for (auto player : players_)
  {
    cout << "  ";
    player->simplePrint();
  }
  cout << endl;
  dungeon_.enterCurrentRoom(0, players_);
  printStoryAndRoom();

  //TODO make better, this is just for testing
}

void Game::step()
{
  if (allPlayersAreDead())
  {
    cout << Game::story_.getStorySegment("N_DEFEAT");
    is_running_ = false;
    printAndSaveScore();
    return;
  }
  else if (dungeon_.isBossDead())
  {
    cout << Game::story_.getStorySegment("N_COMPLETION");
    is_running_ = false;
    printAndSaveScore();
    return;
  }

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
    current_phase_ = Phase::ACTION;
  }
}

void Game::doCommand()
{
  bool command_finished = false;

  cout << Game::story_.getStorySegment("N_PROMPT_MESSAGE");

  while (!command_finished)
  {
    string input = InputOutput::promtUserInput();

    vector<string> command_input = InputOutput::commandifyString(input);

    try
    {
      parser_->execute(command_input);
    }
    catch (const UnknownCommand &e)
    {
      cout << Game::story_.getStorySegment("E_UNKNOWN_COMMAND");
      continue;
    }
    catch (const WrongNumberOfParametersException &e)
    {
      cout << Game::story_.getStorySegment("E_INVALID_PARAM_COUNT");
      continue;
    }
    catch (const InvalidParamCommand &e)
    {
      cout << Game::story_.getStorySegment("E_INVALID_PARAM");
      continue;
    }
    catch (const UnavailableItemOrEntityCommand &e)
    {
      cout << Game::story_.getStorySegment("E_ENTITY_OR_ITEM_UNAVAILABLE");
      continue;
    }
    catch (const InvalidPositionCommand &e)
    {
      cout << Game::story_.getStorySegment("E_INVALID_POSITION");
      continue;
    }
    catch (const CommandExecutionException &e)
    {
      switch (e.getType())
      {
        case CommandExecutionException::ExceptionType::LOCKED_DOOR:
          cout << Game::story_.getStorySegment("E_MOVE_LOCKED_DOOR");
          break;
        case CommandExecutionException::ExceptionType::NO_WEAPON_EQUIPPED:
          cout << Game::story_.getStorySegment("E_ATTACK_NO_WEAPON_EQUIPPED");
          break;
        case CommandExecutionException::ExceptionType::NO_AMMUNITION:
          cout << Game::story_.getStorySegment("E_ATTACK_NO_AMMUNITION");
        default:
          break;
      }
      continue;
    }

    command_finished = true;
  }
}

// TODO: Undo changes
void Game::doCommand(string input)
{
  vector<string> command_input = InputOutput::commandifyString(input);

  bool command_finished = false;

  while (!command_finished)
  {
    try
    {
      parser_->execute(command_input);
    }
    catch (const UnknownCommand &e)
    {
      cout << Game::story_.getStorySegment("E_UNKNOWN_COMMAND");
      return;
    }
    catch (const WrongNumberOfParametersException &e)
    {
      cout << Game::story_.getStorySegment("E_INVALID_PARAM_COUNT");
      return;
    }
    catch (const InvalidParamCommand &e)
    {
      cout << Game::story_.getStorySegment("E_INVALID_PARAM");
      return;
    }
    catch (const UnavailableItemOrEntityCommand &e)
    {
      cout << Game::story_.getStorySegment("E_ENTITY_OR_ITEM_UNAVAILABLE");
      return;
    }
    catch (const InvalidPositionCommand &e)
    {
      cout << Game::story_.getStorySegment("E_INVALID_POSITION");
      return;
    }
    catch (const CommandExecutionException &e)
    {
      switch (e.getType())
      {
        case CommandExecutionException::ExceptionType::LOCKED_DOOR:
          cout << Game::story_.getStorySegment("E_MOVE_LOCKED_DOOR");
          break;
        case CommandExecutionException::ExceptionType::NO_WEAPON_EQUIPPED:
          cout << Game::story_.getStorySegment("E_ATTACK_NO_WEAPON_EQUIPPED");
          break;
        case CommandExecutionException::ExceptionType::NO_AMMUNITION:
          cout << Game::story_.getStorySegment("E_ATTACK_NO_AMMUNITION");
        default:
          break;
      }
      return;
    }

    command_finished = true;
  }
}

bool Game::isRunning() const
{
  return is_running_;
}

bool Game::playerExists(string name) const
{
  for (auto player : players_)
  {
    if (player->getName() == name) { return true; }
  }
  return false;
}

vector<shared_ptr<Player>> Game::getLivingPlayers() const
{
  vector<shared_ptr<Player>> living_players;
  for (auto player : players_)
  {
    if (!player->isDead()) { living_players.push_back(player); }
  }
  return living_players;
}

int Game::getPlayerTypeAmount(char type) const
{
  int count = 0;
  for (auto player : players_)
  {
    if (player->getAbbreviation() == type) { count++; }
  }
  return count;
}

shared_ptr<Player> Game::getPlayerByType(char type) const
{
  for (auto player : players_)
  {
    if (player->getAbbreviation() == type) { return player; }
  }
  return nullptr;
}

shared_ptr<Room> Game::getCurrentRoom() const
{
  return dungeon_.getCurrentRoom();
}

void Game::printStoryAndRoom(bool print_story, bool print_room_completed, bool print_enemy_health)
{
  vector<char> new_enemies =
          Utils::getDifference(dungeon_.getCurrentRoom()->getEnemiesAbbreviations(), dungeon_.getOccuredEnemyTypes());
  Utils::deleteDuplicates(new_enemies);
  if (new_enemies.size() > 0)
  {
    for (auto enemy : new_enemies) { dungeon_.addOccuredEnemyType(enemy); }
  }
  if (!dungeon_.getCurrentRoom()->isComplete() && story_output_active_ && print_story)
  {
    cout << Game::story_.getStorySegment("N_ROOM_" + std::to_string(dungeon_.getCurrentRoom()->getId()));
    if (new_enemies.size() > 0)
    {
      for (auto enemy : new_enemies) { cout << Game::story_.getStorySegment("N_ENEMY_" + string(1, enemy)); }
    }
  }
  dungeon_.getCurrentRoom()->checkCompletion();
  dungeon_.getCurrentRoom()->openDoors();
  if (print_room_completed)
  {
    cout << "\n-- ROOM " << dungeon_.getCurrentRoom()->getId() << " (" << dungeon_.getCompletedRoomsCount() << "/"
         << dungeon_.getRoomCount() << " completed) --------------------\n"
         << endl;
  }
  if (map_output_active_) { dungeon_.printCurrentRoom(); }

  if (print_enemy_health == false) { return; }

  vector<shared_ptr<Character>> enemies = dungeon_.getCurrentRoom()->getEnemies();
  if (enemies.size() > 0)
  {
    cout << "   ";
    for (auto enemy : enemies)
    {
      enemy->simplePrint();
      if (enemy != enemies.back())
      {
        cout << ", ";
      }
      else
      {
        cout << endl;
      }
    }
  }
  else
  {
    cout << endl;
  }
}


bool Game::allPlayersAreDead() const
{
  for (auto player : players_)
  {
    if (!player->isDead()) { return false; }
  }
  return true;
}

void Game::plusOneActionCount()
{
  action_count_++;
  total_actions_++;
}

std::ostream &Game::returnScoreOutput(std::ostream &os)
{
  os << "-- Players --------------------------------------" << endl;
  for (auto player : players_)
  {
    if (!player->isDead())
    {
      os << "  " << player->getTypeName() << " [" << player->getAbbreviation() << "] \"" << player->getName()
         << "\" survived." << endl;
    }
  }
  for (auto player : players_)
  {
    if (player->isDead())
    {
      os << "  " << player->getTypeName() << " [" << player->getAbbreviation() << "] \"" << player->getName()
         << "\", rest in peace." << endl;
    }
  }
  os << "\n-- Statistics -----------------------------------" << endl;
  os << "  " << dungeon_.getCompletedRoomsCount() << " rooms completed" << endl;
  os << "  " << total_actions_ << " performed actions\n" << endl;
  return os;
}

void Game::printAndSaveScore()
{
  returnScoreOutput(cout);
  cout << Game::story_.getStorySegment("N_SCORING_FILE");
  while (true)
  {
    string file_name = InputOutput::promtUserInput();
    if (file_name == "quit") { return; }
    // Open file in write mode and delete content if it already exists
    std::ofstream file(file_name, std::ios::out | std::ios::trunc);
    if (file.is_open())
    {
      returnScoreOutput(file);
      file.close();
      break;
    }
    else
    {
      cout << Game::story_.getStorySegment("E_SCORING_FILE_NOT_WRITABLE");
    }
  }
}

void Game::enemyPhase()
{
  vector<shared_ptr<Enemy>> enemies = dungeon_.getCurrentRoom()->getAllEntitiesOfType<Enemy>();
  if (enemies.empty()) { return; }
  std::sort(enemies.begin(), enemies.end(), [](const shared_ptr<Character> &a, const shared_ptr<Character> &b) -> bool {
    if (a->getAbbreviation() == b->getAbbreviation()) { return a->getId() < b->getId(); }
    return a->getAbbreviation() < b->getAbbreviation();
  });
  for (auto enemy : enemies)
  {
    vector<shared_ptr<Player>> players = dungeon_.getCurrentRoom()->getAllEntitiesOfType<Player>();
    std::sort(players.begin(), players.end(), [this, enemy](const shared_ptr<Player> &a, const shared_ptr<Player> &b) {
      pair<int, int> a_pos = dungeon_.getCurrentRoom()->getFieldOfEntity(a);
      pair<int, int> b_pos = dungeon_.getCurrentRoom()->getFieldOfEntity(b);
      pair<int, int> enemy_pos = dungeon_.getCurrentRoom()->getFieldOfEntity(enemy);

      int distA = std::abs(a_pos.first - enemy_pos.first) + std::abs(a_pos.second - enemy_pos.second);
      int distB = std::abs(b_pos.first - enemy_pos.first) + std::abs(b_pos.second - enemy_pos.second);

      if (distA == distB)
      {
        if (a->getHealth() == b->getHealth())
        {
          // Start code from stackoverflow
          double angleA = atan2(a_pos.second - enemy_pos.second, a_pos.first - enemy_pos.first);
          double angleB = atan2(b_pos.second - enemy_pos.second, b_pos.first - enemy_pos.first);
          // End code from stackoverflow
          return angleA < angleB;
        }
        return a->getHealth() < b->getHealth();
      }
      return distA < distB;
    });
    for (auto player : players)
    {
      pair<int, int> enemy_pos = dungeon_.getCurrentRoom()->getFieldOfEntity(enemy);
      pair<int, int> player_pos = dungeon_.getCurrentRoom()->getFieldOfEntity(player);
      if (dungeon_.getCurrentRoom()->isAdjacentField(enemy_pos, player_pos))
      {
        int damage = enemy->getAttackDamage();
        vector<AttackedField> attacked_fields = getDungeon().characterAttack(enemy, damage, player_pos);
        cout << endl;
        InputOutput::printSuccessFullAttack(enemy, player_pos, attacked_fields);
        InputOutput::printDiceRoll(enemy->getWeapon()->getDice()->getPreviousRoll(), enemy->getWeapon()->getDice());
        InputOutput::printAttackedCharacters(attacked_fields);
        if (allPlayersAreDead()) { return; }
        break;
      }
      else
      {
        int damage = enemy->getAttackDamage(AttackType::RANGED);
        if (damage == -1)
        {
          dungeon_.moveToRandomField(enemy);
          pair<int, int> new_enemy_pos = dungeon_.getCurrentRoom()->getFieldOfEntity(enemy);
          cout << "\n"
               << enemy->getTypeName() << " " << enemy->getId() << " [" << enemy->getAbbreviation() << enemy->getId()
               << "] moved to (" << new_enemy_pos.first << "," << new_enemy_pos.second << ")." << endl;
          break;
        }
        vector<AttackedField> attacked_fields = getDungeon().characterAttack(enemy, damage, player_pos);
        cout << endl;
        InputOutput::printSuccessFullAttack(enemy, player_pos, attacked_fields);
        InputOutput::printDiceRoll(enemy->getWeapon()->getDice()->getPreviousRoll(), enemy->getWeapon()->getDice());
        InputOutput::printAttackedCharacters(attacked_fields);
        if (allPlayersAreDead()) { return; }
        break;
      }
    }
  }
  printStoryAndRoom(false);
}