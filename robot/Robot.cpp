#include "Robot.hpp"
#include "../utility/CSVParser.hpp"

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

    file << getCurrentState().serializeState() << "," << static_cast<int>(action) << "," << q_value << ";";
  }

  file.close();
}

void Robot::loadQTable()
{
  std::vector<std::vector<std::string>> q_table_data = CSVParser::readCSV(q_table_file_path_);

  for (const auto& row : q_table_data) {
    if (row.size() != 3) {
      std::cerr << "Error reading Q-table data: " << q_table_file_path_ << std::endl;
      return;
    }

    State state = State();
    state.deserializeState(row[0]);
    auto action = static_cast<RobotAction>(std::stoi(row[1]));
    double q_value = std::stod(row[2]);

    // q_table_[std::make_tuple(getCurrentState(), static_cast<RobotAction>(action))] = q_value;
  }
}