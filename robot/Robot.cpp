#include "Robot.hpp"
#include "../utility/CSVParser.hpp"
#include "PerformAction.hpp"

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

    q_table_[std::make_tuple(getCurrentState(), static_cast<RobotAction>(action))] = q_value;
  }
}

void Robot::updateQTable(State state, RobotAction action, State new_state, Player player, double reward)
{
  double q_value = q_table_[std::make_tuple(state, action)];
  double max_q_value = getMaximumQValue(new_state, player);
  double new_q_value = q_value + alpha_ * (reward + gamma_ * max_q_value - q_value);
  q_table_[std::make_tuple(state, action)] = new_q_value;
}

RobotAction Robot::getBestAction(State state, Player player)
{
  std::set<RobotAction> possible_actions = state.getPossibleActions(player);
  RobotAction best_action;
  double max_q_value = -std::numeric_limits<double>::infinity();

  // Exploitation: Choose the action with the highest Q-value
  for (const RobotAction& action : possible_actions) {
    double q_value = q_table_[std::make_tuple(state, action)];
    if (q_value > max_q_value) {
      max_q_value = q_value;
      best_action = action;
    }
  }

  // Exploration: With a probability of epsilon_, choose a random action
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> distribution(0, 1);
  if (distribution(gen) < epsilon_) {
    std::uniform_int_distribution<> dis(0, possible_actions.size() - 1);
    auto it = std::next(possible_actions.begin(), dis(gen));
    best_action = *it;
  }

  // Decay epsilon
  epsilon_ = std::max(epsilon_ * epsilon_decay_, epsilon_min_);

  return best_action;
}

void Robot::executeAction(RobotAction action, Player player, std::vector<Player> players)
{
  PerformAction performAction;
  double reward;

  switch (action) {
    case RobotAction::MOVE_UP:
    case RobotAction::MOVE_DOWN:
    case RobotAction::MOVE_LEFT:
    case RobotAction::MOVE_RIGHT:
    case RobotAction::MOVE_UP_LEFT:
    case RobotAction::MOVE_UP_RIGHT:
    case RobotAction::MOVE_DOWN_LEFT:
    case RobotAction::MOVE_DOWN_RIGHT:
      reward = performAction.perform_move(current_state_.getCurrentPosition(), action, current_state_.getDoorPosition());
      break;
    case RobotAction::LOOT:
      reward = performAction.perform_loot(current_state_.getCurrentPosition(), current_state_.getLootables());
      break;
    case RobotAction::REGENERATE:
      reward = performAction.perform_regeneration(player);
      break;
    case RobotAction::SET_RES_FIRE:
    case RobotAction::SET_RES_COLD:
    case RobotAction::SET_RES_FORCE:
    case RobotAction::SET_RES_ACID:
      reward = performAction.perform_resistance(player, action);
      break;
    case RobotAction::ATTACK:
      reward = performAction.perform_attack(player, current_state_.getCurrentPosition(), current_state_.getEnemies());
      break;
    case RobotAction::USE_RANGED:
      reward = performAction.perform_use_ranged(player);
      break;
    case RobotAction::USE_MELEE:
      reward = performAction.perform_use_melee(player);
      break;
    case RobotAction::SWITCH_PLAYER:
      reward = performAction.perform_switch_player(current_state_.getCurrentPlayer(), players);
      break;
    default:
      std::cerr << "Invalid action: " << static_cast<int>(action) << std::endl;
      break;
  }

  // Update Q-table with the reward received from performing the action
  State new_state = current_state_; // Assuming the state has been updated after performing the action
  updateQTable(current_state_, action, new_state, player, reward);
  current_state_ = new_state;
}