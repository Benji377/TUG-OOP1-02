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


class State
{
  char current_player_;
  std::pair<int, int> current_position_;
  int health_;
  int damage_output_; // The amount of damage the robot can deal
  int damage_input_; // The amount of damage the robot can take
  int remaining_enemies_;
  std::map<char, int> distance_to_enemy_; // The distance to each enemy
  std::map<char, int> distance_to_player_; // The distance to each player
  std::map<char, int> distance_to_lootable_; // The distance to each lootable item
  int distance_to_exit_; // The distance to the exit door
  int distance_to_entry_; // The distance to the entry door
  // SET BY THE STATE INTERNALLY
  bool can_attack_range_; // If the robot can attack from a distance
  bool can_attack_melee_; // If the robot can attack in melee
  bool can_heal_; // If the robot can heal itself
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
  State(char current_player, std::pair<int, int> current_position, int health, int remaining_enemies,
        int damage_output, int damage_input, std::map<char, int> distance_to_enemy,
        std::map<char, int> distance_to_player, std::map<char, int> distance_to_lootable,
        int distance_to_exit, int distance_to_entry, std::vector<std::vector<int>> enemies,
        std::vector<std::vector<int>> players, std::vector<std::vector<int>> lootables,
        std::pair<int, int> entry_door_position, std::pair<int, int> exit_door_position);
  State() = default;

  // Getter and Setter methods
  void setCurrentPlayer(char current_player) { current_player_ = current_player; };
  void setCurrentPosition(std::pair<int, int> current_position) { current_position_ = current_position; };
  void setHealth(int health) { health_ = health; };
  void setRemainingEnemies(int remaining_enemies) { remaining_enemies_ = remaining_enemies; };
  void setDamageOutput(int damage_output) { damage_output_ = damage_output; };
  void setDamageInput(int damage_input) { damage_input_ = damage_input; };
  void setCanAttackRange(bool can_attack_range) { can_attack_range_ = can_attack_range; };
  void setCanAttackMelee(bool can_attack_melee) { can_attack_melee_ = can_attack_melee; };
  void setCanHeal(bool can_heal) { can_heal_ = can_heal; };
  void setDistanceToEnemy(std::map<char, int> distance_to_enemy) { distance_to_enemy_ = distance_to_enemy; };
  void setDistanceToPlayer(std::map<char, int> distance_to_player) { distance_to_player_ = distance_to_player; };
  void setDistanceToLootable(std::map<char, int> distance_to_lootable) { distance_to_lootable_ = distance_to_lootable; };
  void setDistanceToExit(int distance) { distance_to_exit_ = distance; };
  void setDistanceToEntry(int distance) { distance_to_entry_ = distance; };
  void setEnemies(std::vector<std::vector<int>> enemies) { enemies_ = enemies; };
  void setPlayers(std::vector<std::vector<int>> players) { players_ = players; };
  void setLootables(std::vector<std::vector<int>> lootables) { lootables_ = lootables; };
  void setEntryDoorPosition(std::pair<int, int> entry_door_position) { entry_door_position_ = entry_door_position; };
  void setExitDoorPosition(std::pair<int, int> exit_door_position) { exit_door_position_ = exit_door_position; };

  char getCurrentPlayer() const { return current_player_; };
  std::pair<int, int> getCurrentPosition() const { return current_position_; };
  int getHealth() const { return health_; };
  int getRemainingEnemies() const { return remaining_enemies_; };
  int getDamageOutput() const { return damage_output_; };
  int getDamageInput() const { return damage_input_; };
  bool getCanAttackRange() const { return can_attack_range_; };
  bool getCanAttackMelee() const { return can_attack_melee_; };
  bool getCanHeal() const { return can_heal_; };
  std::map<char, int> getDistanceToEnemy() const { return distance_to_enemy_; };
  std::map<char, int> getDistanceToPlayer() const { return distance_to_player_; };
  std::map<char, int> getDistanceToLootable() const { return distance_to_lootable_; };
  int getDistanceToExit() const { return distance_to_exit_; };
  int getDistanceToEntry() const { return distance_to_entry_; };

  std::vector<std::vector<int>> getEnemies() const { return enemies_; };
  std::vector<std::vector<int>> getPlayers() const { return players_; };
  std::vector<std::vector<int>> getLootables() const { return lootables_; };
  std::pair<int, int> getEntryDoorPosition() const { return entry_door_position_; };
  std::pair<int, int> getExitDoorPosition() const { return exit_door_position_; };

  // Methods
  std::set<RobotAction> getPossibleActions(Player player);
  std::string serializeState() const;
  void deserializeState(std::string state_string);

  // Overloading some important operators
  bool operator==(const State& state) const;
  bool operator<(const State& state) const;
};


#endif //STATE_HPP
