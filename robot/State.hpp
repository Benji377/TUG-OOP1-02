//---------------------------------------------------------------------------------------------------------------------
//
// The State is a class for Q-learning defining the state of the game at any given time. It holds valuable information
// about the current situation in the game. Technically it's information that a player could see or check with
// display commands, but the AI can't, so we store it here.
// The State needs to be updated every time the game changes, so the AI can learn from it and adapt its strategy.
//
// Group: 068
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef STATE_HPP
#define STATE_HPP

#include <iostream>
#include <vector>
#include <set>
#include "../entity/character/Player.hpp"

// PS: Not all actions are possible at all times, so we will have to filter them and only allow the ones
// that are possible in the current state. This will be done in the Robot class.
enum class RobotAction
{
  UNKNOWN,
  MOVE_UP,
  MOVE_UP_RIGHT, // Diagonally moving to upper right
  MOVE_UP_LEFT, // Diagonally moving to upper left
  MOVE_DOWN,
  MOVE_DOWN_RIGHT, // Diagonally moving to lower right
  MOVE_DOWN_LEFT,  // Diagonally moving to lower left
  MOVE_LEFT,
  MOVE_RIGHT,
  LOOT, // Loots the nearest item (check clockwise)
  REGENERATE,  // Uses a Health potion
  SET_RES_FIRE, // Uses a Resistance potion. Each potion will have a different effect
  SET_RES_COLD,
  SET_RES_FORCE,
  SET_RES_ACID,
  ATTACK, // Attacks the nearest enemy
  USE_RANGED, // Uses a ranged weapon
  USE_MELEE, // Uses a melee weapon
  USE_ARMOR, // Uses an armor item. Only available if it has a better armor than the current one
  SWITCH_PLAYER, // Switches to the next player W -> B -> R
};

// We use the move indicators to tell the robot the direction the closest object is in
enum class MoveIndicator
{
  NONE, // For when there is no object left
  UP,
  UP_RIGHT,
  UP_LEFT,
  DOWN,
  DOWN_RIGHT,
  DOWN_LEFT,
  LEFT,
  RIGHT,
};

enum class HealthIndicator
{
  NONE, // For when there is no object left
  LOW,
  MEDIUM,
  FULL,
};


class State
{
  char current_player_;
  std::pair<int, int> current_position_;
  HealthIndicator health_;
  int max_health_;
  int remaining_enemies_;
  MoveIndicator distance_to_exit_; // The distance to the exit door
  MoveIndicator distance_to_entry_; // The distance to the entry door
  MoveIndicator distance_to_closest_enemy_;
  bool can_attack_range_; // If the robot can attack from a distance
  bool can_attack_melee_; // If the robot can attack in melee
  std::pair<int, int> distance_to_target_; // The distance to the target
  // The following is not saved in the state, but used for calculating the possible actions
  std::vector<std::vector<int>> enemies_; // The position of the enemies
  std::vector<std::vector<int>> players_; // The position of the players
  std::vector<std::vector<int>> lootables_; // The position of the lootable items
  std::pair<int, int> entry_door_position_; // The position of the entry door
  std::pair<int, int> exit_door_position_; // The position of the exit door

  // Methods
  bool canLoot();
  bool canRegenerate(Player player);
  std::set<RobotAction> getPossibleResistances(Player player);
  bool canAttack(Player player);
  bool canAttackAdjacent(Player player);
  bool canAttackAnywhere(Player player);
  bool canUseRanged(Player player);
  bool canUseMelee(Player player);
  bool canSwitchPlayer();
  bool canUseArmor(Player player);
  std::set<RobotAction> getPossibleMoves();


public:
  State(char current_player, std::pair<int, int> current_position, int health, int max_health,
        std::vector<std::vector<int>> enemies, std::vector<std::vector<int>> players,
        std::vector<std::vector<int>> lootables, std::pair<int, int> entry_door_position,
        std::pair<int, int> exit_door_position);
  State() = default;

  // Getter and Setter methods
  void setCurrentPlayer(char current_player) { current_player_ = current_player; };
  void setCurrentPosition(std::pair<int, int> current_position) { current_position_ = current_position; };
  void setRemainingEnemies(int remaining_enemies) { remaining_enemies_ = remaining_enemies; };
  void setCanAttackRange(bool can_attack_range) { can_attack_range_ = can_attack_range; };
  void setCanAttackMelee(bool can_attack_melee) { can_attack_melee_ = can_attack_melee; };
  void setEnemies(std::vector<std::vector<int>> enemies) { enemies_ = enemies; };
  void setPlayers(std::vector<std::vector<int>> players) { players_ = players; };
  void setLootables(std::vector<std::vector<int>> lootables) { lootables_ = lootables; };
  void setEntryDoorPosition(std::pair<int, int> entry_door_position) { entry_door_position_ = entry_door_position; };
  void setExitDoorPosition(std::pair<int, int> exit_door_position) { exit_door_position_ = exit_door_position; };
  void setDistanceToClosestEnemy(MoveIndicator distance_to_closest_enemy) { distance_to_closest_enemy_ = distance_to_closest_enemy; };
  void setDistanceToExit(MoveIndicator distance_to_exit) { distance_to_exit_ = distance_to_exit; };
  void setDistanceToTarget(std::pair<int, int> distance_to_target) { distance_to_target_ = distance_to_target; };
  void setHealth(HealthIndicator health_indicator) { health_ = health_indicator; };
  void setMaxHealth(int max_health) { max_health_ = max_health; };

  char getCurrentPlayer() const { return current_player_; };
  std::pair<int, int> getCurrentPosition() const { return current_position_; };
  HealthIndicator getHealth() const { return health_; };
  int getRemainingEnemies() const { return remaining_enemies_; };
  bool getCanAttackRange() const { return can_attack_range_; };
  bool getCanAttackMelee() const { return can_attack_melee_; };
  MoveIndicator getDistanceToClosestEnemy() const { return distance_to_closest_enemy_; };
  MoveIndicator getDistanceToExit() const { return distance_to_exit_; };
  MoveIndicator getDistanceToEntry() const { return distance_to_entry_; };
  std::pair<int, int> getDistanceToTarget() const { return distance_to_target_; };

  std::vector<std::vector<int>> getEnemies() const { return enemies_; };
  std::vector<std::vector<int>> getPlayers() const { return players_; };
  std::vector<std::vector<int>> getLootables() const { return lootables_; };
  std::pair<int, int> getEntryDoorPosition() const { return entry_door_position_; };
  std::pair<int, int> getExitDoorPosition() const { return exit_door_position_; };
  int getMaxHealth() const { return max_health_; };

  // Methods
  std::set<RobotAction> getPossibleActions(Player player);
  std::string serializeState() const;
  void deserializeState(std::string state_string);
  MoveIndicator getMoveIndicator(std::pair<int, int> position) const;
  HealthIndicator getHealthIndicator(int health) const;
  void initializeVars(int health);
  void updateState(char current_player, std::pair<int, int> current_position, int health, int max_health,
                   std::vector<std::vector<int>> enemies, std::vector<std::vector<int>> players,
                   std::vector<std::vector<int>> lootables, std::pair<int, int> entry_door_position,
                   std::pair<int, int> exit_door_position);


  // Overloading some important operators
  bool operator==(const State& state) const;
  bool operator<(const State& state) const;
};


#endif //STATE_HPP
