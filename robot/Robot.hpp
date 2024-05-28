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

// TODO: We might also put some helper methods in a RobotHelper class in the same folder to organize the code better
class Robot
{

};


#endif //ROBOT_HPP
