// Helper class for the robot

#ifndef PERFORM_ACTION_HPP
#define PERFORM_ACTION_HPP

#include "State.hpp"

class PerformAction
{
public:
  PerformAction() = default;
  double perform_move(std::pair<int, int> player_position, RobotAction action, std::pair<int, int> door_position);
  double perform_loot(std::pair<int, int> player_position, std::vector<std::vector<int>> lootables);
  double perform_regeneration(Player player);
  double perform_resistance(Player player, RobotAction action);
  double perform_attack(Player player, std::pair<int, int> player_position, std::vector<std::vector<int>> enemies);
  double perform_use_ranged(Player player);
  double perform_use_melee(Player player);
  double perform_switch_player(char current_player, std::vector<Player> players);
  double perform_use_armor(Player player);
};


#endif //PERFORM_ACTION_HPP
