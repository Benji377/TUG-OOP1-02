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
#include <map>
#include <tuple>
#include <fstream>
#include <sstream>
#include "State.hpp"

// TODO: We might also put some helper methods in a RobotHelper class in the same folder to organize the code better
class Robot
{
  State current_state_;
  std::map<std::tuple<State, RobotAction>, double> q_table_;
  static const std::string q_table_file_path_;
  // TODO: The following numbers will have to be adjusted and tested to find the best values
  double alpha_ = 0.1;
  double gamma_ = 0.95;
  double epsilon_ = 0.9;
  double epsilon_decay_ = 0.995;
  double epsilon_min_ = 0.01;

public:
  explicit Robot(State state);
  void saveQTable();
  void loadQTable();
  void updateQTable(State state, RobotAction action, State new_state, double reward);
  RobotAction getBestAction(State state);
  void executeAction(RobotAction action);
  double getMaximumQValue(State state);
  State getCurrentState();
  void setCurrentState(State state);

};


#endif //ROBOT_HPP
