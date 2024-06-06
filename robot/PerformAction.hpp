// Helper class for the robot

#ifndef PERFORM_ACTION_HPP
#define PERFORM_ACTION_HPP

#include "State.hpp"

class Game;
static const double REWARD_EXCEPTION = -100.0;
static const double REWARD_MOVE = -1.0;
static const double REWARD_MOVE_DOOR_ENTRY = -50.0;
static const double REWARD_MOVE_DOOR_EXIT = 100.0;
static const double REWARD_LOOT = 10.0;
static const double REWARD_HEAL_FULL = -100.0;
static const double REWARD_HEAL_HALF = 10.0;
static const double REWARD_HEAL_SOME = 5.0;
static const double REWARD_USE_RESISTANCE_FULL = 1.0;
static const double REWARD_USE_RESISTANCE_SOME = 10.0;
static const double REWARD_ATTACK = 100.0;
static const double REWARD_ENEMY_KILLED = 100.0;
static const double REWARD_PLAYER_HIT = -100.0;
static const double REWARD_PLAYER_KILLED = -100.0;
static const double REWARD_EQUIP_WEAPON = 1.0;
static const double REWARD_EQUIP_ARMOR = 1.0;
static const double REWARD_SWITCH_PLAYER = 1.0;

class PerformAction
{
public:
  Game* game_;
  explicit PerformAction(Game* game) : game_(game) {}
  double perform_move(Player player, std::pair<int, int> player_position, RobotAction action,
                      std::pair<int, int> entry_door_position, std::pair<int, int> exit_door_position, int enemies_left);
  double perform_loot(Player player, std::pair<int, int> player_position, std::vector<std::vector<int>> lootables);
  double perform_regeneration(Player player);
  double perform_resistance(Player player, RobotAction action);
  double perform_attack(Player player, std::pair<int, int> player_position, std::vector<std::vector<int>> enemies);
  double perform_use_ranged(Player player);
  double perform_use_melee(Player player);
  double perform_switch_player(char current_player, std::vector<Player> players);
  double perform_use_armor(Player player);
};


#endif //PERFORM_ACTION_HPP
