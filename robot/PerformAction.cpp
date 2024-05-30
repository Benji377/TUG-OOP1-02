#include "PerformAction.hpp"

double PerformAction::perform_move(std::pair<int, int> player_position, RobotAction action, std::pair<int, int> door_position)
{
  std::map<RobotAction, std::pair<int, int>> actionToMove = {
          {RobotAction::MOVE_UP, {0, -1}},
          {RobotAction::MOVE_DOWN, {0, 1}},
          {RobotAction::MOVE_LEFT, {-1, 0}},
          {RobotAction::MOVE_RIGHT, {1, 0}},
          {RobotAction::MOVE_UP_LEFT, {-1, -1}},
          {RobotAction::MOVE_UP_RIGHT, {1, -1}},
          {RobotAction::MOVE_DOWN_LEFT, {-1, 1}},
          {RobotAction::MOVE_DOWN_RIGHT, {1, 1}}
  };

  if (actionToMove.count(action))
  {
    int new_x = player_position.first + actionToMove[action].first;
    int new_y = player_position.second + actionToMove[action].second;
    // TODO Replace with actual command to move the player
    std::cout << "Robot moved to: (" << new_x << ", " << new_y << ")" << std::endl;
    if (new_x >= 0 && new_x < door_position.first && new_y >= 0 && new_y < door_position.second)
    {
      std::cout << "Robot reached the door!" << std::endl;
      return 100.0;
    }
    return -1.0;
  }
  // If the action is not a move action, return a big negative number
  return -100.0;
}

double PerformAction::perform_loot(std::pair<int, int> player_position, std::vector<std::vector<int>> lootables)
{
  // Define the offsets for the surrounding cells
  std::vector<std::pair<int, int>> offsets = {{0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  for (const auto& offset : offsets) {
    int new_x = player_position.first + offset.first;
    int new_y = player_position.second + offset.second;

    // Check if the new position is within the bounds of the lootables vector
    if (new_x >= 0 && new_x < lootables.size() && new_y >= 0 && new_y < lootables[0].size()) {
      if (lootables[new_y][new_x] == 1) {
        // TODO Replace with actual command to loot the player
        std::cout << "Robot looted at: (" << new_x << ", " << new_y << ")" << std::endl;
        return 10.0;
      }
    }
  }

  // If there is no loot, return a big negative number
  return -100.0;
}

double PerformAction::perform_regeneration(Player player)
{
  // TODO Replace with actual command to regenerate the player
  std::cout << "Robot used health potion" << std::endl;
  return 1.0;
}

double PerformAction::perform_resistance(Player player, RobotAction action)
{
  // Define the resistance types
  std::map<RobotAction, std::string> resistanceTypes = {
          {RobotAction::SET_RES_FIRE, "Fire"},
          {RobotAction::SET_RES_COLD, "Cold"},
          {RobotAction::SET_RES_FORCE, "Force"},
          {RobotAction::SET_RES_ACID, "Acid"}
  };

  if (resistanceTypes.count(action))
  {
    // TODO Replace with actual command to set resistance
    std::cout << "Robot used " << resistanceTypes[action] << " resistance potion" << std::endl;
    return 1.0;
  }

  // If the action is not a resistance action, return a big negative number
  return -100.0;
}

double PerformAction::perform_attack(Player player, std::pair<int, int> player_position, std::vector<std::vector<int>> enemies)
{
  // TODO: Add a big reward if the enemy is killed
  if (player.getWeapon()->getAttackType() == AttackType::MELEE)
  {
    // Define the offsets for the surrounding cells
    std::vector<std::pair<int, int>> offsets = {{0,  0},
                                                {-1, 0},
                                                {1,  0},
                                                {0,  -1},
                                                {0,  1},
                                                {-1, -1},
                                                {-1, 1},
                                                {1,  -1},
                                                {1,  1}};

    for (const auto &offset: offsets)
    {
      int new_x = player_position.first + offset.first;
      int new_y = player_position.second + offset.second;

      // Check if the new position is within the bounds of the enemies vector
      if (new_x >= 0 && new_x < enemies.size() && new_y >= 0 && new_y < enemies[0].size())
      {
        if (enemies[new_y][new_x] > 0)
        {
          // TODO Replace with actual command to attack the enemy
          std::cout << "Robot attacked enemy at: (" << new_x << ", " << new_y << ") using melee weapon" << std::endl;
          return 10.0;
        }
      }
    }
  } else {
    // If the player has a ranged weapon, check if there is an enemy in range
    for (int i = 0; i < enemies.size(); i++)
    {
      for (int j = 0; j < enemies[0].size(); j++)
      {
        if (enemies[i][j] > 0)
        {
          // TODO Replace with actual command to attack the enemy
          std::cout << "Robot attacked enemy at: (" << j << ", " << i << ") using ranged weapon" << std::endl;
          return 10.0;
        }
      }
    }
  }

  // If there is no enemy to attack, return a big negative number
  return -100.0;
}

double PerformAction::perform_use_ranged(Player player)
{
  // TODO Replace with actual command to use ranged weapon
  std::cout << "Robot equipped ranged weapon" << std::endl;
  return 1.0;
}

double PerformAction::perform_use_melee(Player player)
{
  // TODO Replace with actual command to use melee weapon
  std::cout << "Robot equipped melee weapon" << std::endl;
  return 1.0;
}

double PerformAction::perform_switch_player(char current_player, std::vector<Player> players)
{
  // Find the index of the current player
  int current_player_index = -1;
  for (int i = 0; i < players.size(); i++)
  {
    if (players[i].getAbbreviation() == current_player)
    {
      current_player_index = i;
      break;
    }
  }

  if (current_player_index != -1)
  {
    // Switch to the next player in the list
    int next_player_index = (current_player_index + 1) % players.size();
    // TODO Replace with actual command to switch player
    std::cout << "Switched to player: " << players[next_player_index].getAbbreviation() << std::endl;
    return 1.0;
  }

  // If the player is not found, return a big negative number
  return -100.0;
}
