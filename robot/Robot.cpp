#include "Robot.hpp"
#include "../utility/CSVParser.hpp"
#include "PerformAction.hpp"

const std::string Robot::q_table_file_path_ = "data/q_table.dat";

// DEBUG ONLY
std::string getRobotActionAsString(RobotAction action) {
  switch(action) {
    case RobotAction::MOVE_UP: return "MOVE_UP";
    case RobotAction::MOVE_UP_RIGHT: return "MOVE_UP_RIGHT";
    case RobotAction::MOVE_UP_LEFT: return "MOVE_UP_LEFT";
    case RobotAction::MOVE_DOWN: return "MOVE_DOWN";
    case RobotAction::MOVE_DOWN_RIGHT: return "MOVE_DOWN_RIGHT";
    case RobotAction::MOVE_DOWN_LEFT: return "MOVE_DOWN_LEFT";
    case RobotAction::MOVE_LEFT: return "MOVE_LEFT";
    case RobotAction::MOVE_RIGHT: return "MOVE_RIGHT";
    case RobotAction::LOOT: return "LOOT";
    case RobotAction::REGENERATE: return "REGENERATE";
    case RobotAction::SET_RES_FIRE: return "SET_RES_FIRE";
    case RobotAction::SET_RES_COLD: return "SET_RES_COLD";
    case RobotAction::SET_RES_FORCE: return "SET_RES_FORCE";
    case RobotAction::SET_RES_ACID: return "SET_RES_ACID";
    case RobotAction::ATTACK: return "ATTACK";
    case RobotAction::USE_RANGED: return "USE_RANGED";
    case RobotAction::USE_MELEE: return "USE_MELEE";
    case RobotAction::USE_ARMOR: return "USE_ARMOR";
    case RobotAction::SWITCH_PLAYER: return "SWITCH_PLAYER";
    default: return "UNKNOWN_ACTION";
  }
}

Robot::Robot(State state, Game* game) : current_state_(state), game_(game)
{
  loadQTable();
}

void Robot::saveQTable() {
  try {
    std::ofstream file(q_table_file_path_, std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "Error opening file for writing: " << q_table_file_path_ << std::endl;
      return;
    }

    size_t q_table_size = q_table_.size();
    file.write(reinterpret_cast<const char*>(&q_table_size), sizeof(q_table_size));

    for (const auto& entry : q_table_) {
      const auto& state_action = entry.first;
      const State& state = std::get<0>(state_action);
      std::string state_data = state.serializeState();
      RobotAction action = std::get<1>(state_action);
      double q_value = entry.second;

      size_t state_size = state_data.size();
      file.write(reinterpret_cast<const char*>(&state_size), sizeof(state_size));
      file.write(state_data.c_str(), state_size);
      file.write(reinterpret_cast<const char*>(&action), sizeof(action));
      file.write(reinterpret_cast<const char*>(&q_value), sizeof(q_value));
    }

    file.close();
  } catch (const std::bad_alloc& e) {
    std::cerr << "Memory allocation failed: " << e.what() << std::endl;
  }
}

void Robot::loadQTable() {
  try {
    std::ifstream file(q_table_file_path_, std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "File not found, initializing Q-table with zeros: " << q_table_file_path_ << std::endl;
      return;
    }

    size_t q_table_size;
    file.read(reinterpret_cast<char*>(&q_table_size), sizeof(q_table_size));

    for (size_t i = 0; i < q_table_size; ++i) {
      size_t state_size;
      file.read(reinterpret_cast<char*>(&state_size), sizeof(state_size));

      std::string state_data(state_size, '\0');
      file.read(&state_data[0], state_size);

      State state;
      state.deserializeState(state_data);

      RobotAction action;
      file.read(reinterpret_cast<char*>(&action), sizeof(action));

      double q_value;
      file.read(reinterpret_cast<char*>(&q_value), sizeof(q_value));

      q_table_[std::make_tuple(state, action)] = q_value;
    }

    file.close();
  } catch (const std::bad_alloc& e) {
    std::cerr << "Memory allocation failed: " << e.what() << std::endl;
  }
}


void Robot::updateQTable(RobotAction action, Player player, double reward)
{
  State state = getPreviousState();
  State new_state = getCurrentState();

  double q_value = q_table_[std::make_tuple(state, action)];
  double max_q_value = getMaximumQValue(new_state, player);
  double new_q_value = q_value + alpha_ * (reward + gamma_ * max_q_value - q_value);
  q_table_[std::make_tuple(state, action)] = new_q_value;

  saveQTable();
}

RobotAction Robot::getBestAction(State state, Player player)
{
  std::cout << "[DEBUG] Getting best action for player: " << player.getName() << std::endl;
  std::set<RobotAction> possible_actions = state.getPossibleActions(player);
  // Output possible actions
  std::cout << "[DEBUG] Possible actions: ";
  for (const RobotAction& action : possible_actions) {
    std::cout << getRobotActionAsString(action) << ", ";
  }
  std::cout << std::endl;
  RobotAction best_action = RobotAction::UNKNOWN;
  double max_q_value = -std::numeric_limits<double>::infinity();
  std::cout << "[DEBUG] Max Q-value: " << max_q_value << std::endl;

  // Exploitation: Choose the action with the highest Q-value
  for (const RobotAction& action : possible_actions) {
    auto it = q_table_.find(std::make_tuple(state, action));
    if (it != q_table_.end()) {
      double q_value = it->second;
      std::cout << "[DEBUG] Q-value for action " << getRobotActionAsString(action) << ": " << q_value << std::endl;
      if (q_value > max_q_value) {
        max_q_value = q_value;
        best_action = action;
      }
    }
  }

  // Exploration: With a probability of epsilon_, choose a random action
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> distribution(0, 1);
  if (distribution(gen) < epsilon_ || best_action == RobotAction::UNKNOWN) {
    std::cout << "[DEBUG] Exploration: Choosing random action" << std::endl;
    std::uniform_int_distribution<> dis(0, possible_actions.size() - 1);
    auto it = std::next(possible_actions.begin(), dis(gen));
    best_action = *it;
  }
  else
  {
    std::cout << "[DEBUG] Exploitation: Choosing best action" << std::endl;
  }
  std::cout << "[DEBUG] Best action: (" << static_cast<int>(best_action) << ") " << getRobotActionAsString(best_action) << std::endl;

  // Decay epsilon
  epsilon_ = std::max(epsilon_ * epsilon_decay_, epsilon_min_);
  std::cout << "[DEBUG] Epsilon decayed: " << epsilon_ << std::endl;

  return best_action;
}

double Robot::executeAction(RobotAction action, Player player, std::vector<Player> players)
{
  PerformAction performAction = PerformAction(game_);

  switch (action) {
    case RobotAction::MOVE_UP:
    case RobotAction::MOVE_DOWN:
    case RobotAction::MOVE_LEFT:
    case RobotAction::MOVE_RIGHT:
    case RobotAction::MOVE_UP_LEFT:
    case RobotAction::MOVE_UP_RIGHT:
    case RobotAction::MOVE_DOWN_LEFT:
    case RobotAction::MOVE_DOWN_RIGHT:
      return performAction.perform_move(player, current_state_.getCurrentPosition(), action,
                                        current_state_.getEntryDoorPosition(), current_state_.getExitDoorPosition(),
                                        current_state_.getRemainingEnemies());
    case RobotAction::LOOT:
      return performAction.perform_loot(player, current_state_.getCurrentPosition(), current_state_.getLootables());
    case RobotAction::REGENERATE:
      return performAction.perform_regeneration(player);
    case RobotAction::SET_RES_FIRE:
    case RobotAction::SET_RES_COLD:
    case RobotAction::SET_RES_FORCE:
    case RobotAction::SET_RES_ACID:
      return performAction.perform_resistance(player, action);
    case RobotAction::ATTACK:
      return performAction.perform_attack(player, current_state_.getCurrentPosition(), current_state_.getEnemies());
    case RobotAction::USE_RANGED:
      return performAction.perform_use_ranged(player);
    case RobotAction::USE_MELEE:
      return performAction.perform_use_melee(player);
    case RobotAction::SWITCH_PLAYER:
      return performAction.perform_switch_player(current_state_.getCurrentPlayer(), players);
    case RobotAction::USE_ARMOR:
      return performAction.perform_use_armor(player);
    default:
      std::cout << "Invalid action: " << getRobotActionAsString(action) << std::endl;
      throw std::invalid_argument("Invalid action");
  }
}

double Robot::getMaximumQValue(State state, Player player)
{
  std::set<RobotAction> possible_actions = state.getPossibleActions(player);
  double max_q_value = -std::numeric_limits<double>::infinity();

  for (const RobotAction& action : possible_actions) {
    auto it = q_table_.find(std::make_tuple(state, action));
    if (it != q_table_.end()) {
      double q_value = it->second;
      if (q_value > max_q_value) {
        max_q_value = q_value;
      }
    }
  }

  // If max_q_value is still negative infinity, return 0 instead
  if (max_q_value == -std::numeric_limits<double>::infinity()) {
    return 0;
  }

  return max_q_value;
}

