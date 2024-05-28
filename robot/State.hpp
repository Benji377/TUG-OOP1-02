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

// TODO: Specify the Actions in more detail, to make each decision more precise and easier to handle
// PS: Not all actions are possible at all times, so we will have to filter them and only allow the ones
// that are possible in the current state. This will be done in the Robot class.
enum class RobotAction
{
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
  ATTACK,
  USE_RANGED, // Uses a ranged weapon
  USE_MELEE, // Uses a melee weapon
};


class State
{
  std::pair<int, int> current_position_;
  int health_;
  int remaining_action_count_;
  int damage_output_; // The amount of damage the robot can deal
  int damage_input_; // The amount of damage the robot can take
  bool can_attack_range_; // If the robot can attack from a distance
  bool can_attack_melee_; // If the robot can attack in melee
  bool can_heal_; // If the robot can heal itself
  std::vector<std::vector<int>> enemies_; // map of the game, where 0 is empty, everything > 1 is the health of the enemy
  std::vector<std::vector<int>> players_; // map of the game, where 0 is empty, everything > 1 is the health of the player
  std::vector<std::vector<int>> lootables_; // map of the game, where 0 is empty, 1 is a lootable item
  int door_pos_x_, door_pos_y_; // The position of the door in the room

public:
  State();

  // Getter and Setter methods
  void setCurrentPosition(std::pair<int, int> current_position) { current_position_ = current_position; };
  void setHealth(int health) { health_ = health; };
  void setRemainingActionCount(int remaining_action_count) { remaining_action_count_ = remaining_action_count; };
  void setDamageOutput(int damage_output) { damage_output_ = damage_output; };
  void setDamageInput(int damage_input) { damage_input_ = damage_input; };
  void setCanAttackRange(bool can_attack_range) { can_attack_range_ = can_attack_range; };
  void setCanAttackMelee(bool can_attack_melee) { can_attack_melee_ = can_attack_melee; };
  void setCanHeal(bool can_heal) { can_heal_ = can_heal; };
  void setEnemies(std::vector<std::vector<int>> enemies) { enemies_ = enemies; };
  void setPlayer(std::vector<std::vector<int>> players) { players_ = players; };
  void setLootables(std::vector<std::vector<int>> lootables) { lootables_ = lootables; };
  void setDoorPosition(int x, int y) { door_pos_x_ = x; door_pos_y_ = y; };

  std::pair<int, int> getCurrentPosition() { return current_position_; };
  int getHealth() { return health_; };
  int getRemainingActionCount() { return remaining_action_count_; };
  int getDamageOutput() { return damage_output_; };
  int getDamageInput() { return damage_input_; };
  bool getCanAttackRange() { return can_attack_range_; };
  bool getCanAttackMelee() { return can_attack_melee_; };
  bool getCanHeal() { return can_heal_; };
  std::vector<std::vector<int>> getEnemies() { return enemies_; };
  std::vector<std::vector<int>> getPlayers() { return players_; };
  std::vector<std::vector<int>> getLootables() { return lootables_; };
  std::pair<int, int> getDoorPosition() { return std::make_pair(door_pos_x_, door_pos_y_); };

  // Methods to update the state
  void serializeState(); // Serializes the state to a file
  void deserializeState(); // Deserializes the state from a file
  std::vector<RobotAction> getPossibleActions();
};


#endif //STATE_HPP
