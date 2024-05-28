//---------------------------------------------------------------------------------------------------------------------
//
// The Robot class is technically our AI. It contains the Q-Learning algorithm and the logic for the robot to move.
// Especially it contains the Q-table and the logic to update it. It also has an Enum containing all the possible
// actions the robot can take. The actions are filtered by the environment, so the robot can only take actions that
// are possible in the current state. Furthermore, the robot won't call commands directly, but instead it will call
// simplified functions which will then call the commands for it. This is to make the robot more modular and easier
// to change in the future, while also making it easier to test and less prone to errors.
//
// Group: 068
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <iostream>

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
  LOOT, // This may automatically use the item if it is better, or we could let the robot decide what to do with it
  REGENERATE,  // Uses a Health potion
  SET_RESISTANCE, // Uses a Resistance potion. Maybe do an action for each resistance type?
  ATTACK, // If we don't allow the robot to do "use", he might never equip a ranged weapon to attack or vice versa
};

// TODO: We might also put some helper methods in a RobotHelper class in the same folder to organize the code better
class Robot
{

};


#endif //ROBOT_HPP
