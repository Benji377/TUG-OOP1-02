#include "Robot.hpp"

const std::string Robot::q_table_file_path_ = "data/q_table.csv";

Robot::Robot(State state) : current_state_(state)
{
  loadQTable();
}

void Robot::saveQTable()
{
  std::ofstream file(q_table_file_path_);
  if (!file.is_open()) {
    std::cerr << "Error opening file for writing: " << q_table_file_path_ << std::endl;
    return;
  }

  for (const auto& entry : q_table_) {
    const auto& state_action = entry.first;
    const State& state = std::get<0>(state_action);
    RobotAction action = std::get<1>(state_action);
    double q_value = entry.second;

    file << getCurrentState().serializeState() << "," << static_cast<int>(action) << "," << q_value << std::endl;
  }

  file.close();
}