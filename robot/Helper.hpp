// Helper class for the robot

#ifndef HELPER_HPP
#define HELPER_HPP

#include "State.hpp"

class Helper
{
public:
  Helper() = default;
  double perform_move(std::pair<int, int> player_position, RobotAction action);
  double perform_loot(std::pair<int, int> player_position);
  double perform_regeneration(Player player);
  double perform_resistance(Player player, RobotAction action);
  double perform_attack(Player player, std::pair<int, int> player_position);
  double perform_use_ranged(Player player);
  double perform_use_melee(Player player);
  double perform_switch_player(char current_player, std::vector<Player> players);
};


#endif //HELPER_HPP
