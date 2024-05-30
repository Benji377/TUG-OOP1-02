#include "State.hpp"

std::set<RobotAction> State::getPossibleActions(Player player)
{
  // We use a std::set to avoid duplicates
  std::set<RobotAction> possible_actions;
  // we add all possible moves to the set
  for (const auto& move : getPossibleMoves()) {
    possible_actions.insert(move);
  }
  if (canLoot()) {
    possible_actions.insert(RobotAction::LOOT);
  }
  if (canRegenerate(player)) {
    possible_actions.insert(RobotAction::REGENERATE);
  }
  for (const auto& resistance : getPossibleResistances(player)) {
    possible_actions.insert(resistance);
  }
  if (canAttack(player)) {
    possible_actions.insert(RobotAction::ATTACK);
  }
  if (canUseRanged(player)) {
    possible_actions.insert(RobotAction::USE_RANGED);
  }
  if (canUseMelee(player)) {
    possible_actions.insert(RobotAction::USE_MELEE);
  }
  if (canSwitchPlayer()) {
    possible_actions.insert(RobotAction::SWITCH_PLAYER);
  }
  return possible_actions;
}

std::set<RobotAction> State::getPossibleMoves()
{
  std::set<RobotAction> possible_moves;
  std::vector<std::pair<RobotAction, std::pair<int, int>>> moves = {
          {RobotAction::MOVE_RIGHT, {1, 0}},
          {RobotAction::MOVE_LEFT, {-1, 0}},
          {RobotAction::MOVE_DOWN, {0, 1}},
          {RobotAction::MOVE_UP, {0, -1}},
          {RobotAction::MOVE_DOWN_RIGHT, {1, 1}},
          {RobotAction::MOVE_UP_LEFT, {-1, -1}},
          {RobotAction::MOVE_DOWN_LEFT, {-1, 1}},
          {RobotAction::MOVE_UP_RIGHT, {1, -1}}
  };
  // In this simplified version, the possible moves and their corresponding changes in position
  // are defined in the moves vector. The code then iterates over these moves,
  // calculates the new position for each move, and checks if the move is valid
  // (i.e., within the room and not occupied by an enemy). If the move is valid, it is added to the possible_moves set.
  // This reduces the number of conditional checks and makes the code more concise and easier to understand.
  for (const auto& move : moves) {
    int new_x = getCurrentPosition().first + move.second.first;
    int new_y = getCurrentPosition().second + move.second.second;
    if (new_x >= 0 && new_x < getEnemies().size() && new_y >= 0 && new_y < getEnemies()[0].size() &&
        getEnemies()[new_x][new_y] == 0) {
      possible_moves.insert(move.first);
    }
  }

  return possible_moves;
}

bool State::canLoot() {
  // Looking at the items map and the player position. If there is an item in the player position
  // or in the 8 adjacent positions, the player can loot.
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int new_x = getCurrentPosition().first + i;
      int new_y = getCurrentPosition().second + j;
      if (new_x >= 0 && new_x < getLootables().size() && new_y >= 0 && new_y < getLootables()[0].size() &&
          getLootables()[new_x][new_y] == 1) {
        return true;
      }
    }
  }
  return false;
}

bool State::canRegenerate(Player player)
{
  // The player can regenerate if it has lost health and there is a health potion in his inventory
  if (!player.getInventory()->getAllPotions().empty()) {
    for (const auto& potion : player.getInventory()->getAllPotions()) {
      if (potion->getEffect() == Effect::HEALTH && player.getHealth() < player.getMaximumHealth()) {
        return true;
      }
    }
  }
  return false;
}

std::set<RobotAction> State::getPossibleResistances(Player player)
{
  std::set<RobotAction> possible_resistances;
  std::map<Effect, RobotAction> effectToAction = {
          {Effect::FIRE, RobotAction::SET_RES_FIRE},
          {Effect::COLD, RobotAction::SET_RES_COLD},
          {Effect::FORCE, RobotAction::SET_RES_FORCE},
          {Effect::ACID, RobotAction::SET_RES_ACID}
  };

  for (const auto& potion : player.getInventory()->getAllPotions()) {
    if (effectToAction.count(potion->getEffect())) {
      possible_resistances.insert(effectToAction[potion->getEffect()]);
    }
  }

  return possible_resistances;
}

bool State::canAttack(Player player)
{
  if (hasEnemies() && (canAttackAdjacent(player) || canAttackAnywhere(player))) {
    return true;
  }
  return false;
}

bool State::hasEnemies()
{
  for (const auto& row : getEnemies()) {
    if (std::any_of(row.begin(), row.end(), [](int cell) { return cell > 0; })) {
      return true;
    }
  }
  return false;
}

bool State::canAttackAdjacent(Player player)
{
  if (player.getWeapon()->getAttackType() == AttackType::MELEE) {
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        int new_x = getCurrentPosition().first + i;
        int new_y = getCurrentPosition().second + j;
        if (new_x >= 0 && new_x < getEnemies().size() && new_y >= 0 && new_y < getEnemies()[0].size() &&
            getEnemies()[new_x][new_y] > 0) {
          return true;
        }
      }
    }
  }
  return false;
}

bool State::canAttackAnywhere(Player player)
{
  if (player.getWeapon()->getAttackType() == AttackType::RANGED) {
    if (player.getWeapon()->getAbbreviation().find('Q') != std::string::npos) {
      return true;
    } else {
      std::vector<std::shared_ptr<Ammunition>> ammunition = player.getInventory()->getAllAmmunition();
      if (!ammunition.empty()) {
        for (const auto& ammo : ammunition) {
          std::vector<std::string> ammo_weapon = ammo->getWeapons();
          if (std::find(ammo_weapon.begin(), ammo_weapon.end(), player.getWeapon()->getAbbreviation()) != ammo_weapon.end()) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool State::canUseRanged(Player player)
{
  // Can use ranged weapon if the player has a ranged weapon in his inventory
  if (!player.getInventory()->getAllWeapons().empty())
  {
    for (const auto& weapon : player.getInventory()->getAllWeapons())
    {
      if (weapon->getAttackType() == AttackType::RANGED)
      {
        return true;
      }
    }
  }
  return false;
}

bool State::canUseMelee(Player player)
{
  // Can use melee weapon if the player has a melee weapon in his inventory
  if (!player.getInventory()->getAllWeapons().empty())
  {
    for (const auto& weapon : player.getInventory()->getAllWeapons())
    {
      if (weapon->getAttackType() == AttackType::MELEE)
      {
        return true;
      }
    }
  }
  return false;
}

bool State::canSwitchPlayer()
{
  // The robot can switch player if there are more than one player in the game
  for (int i = 0; i < getPlayers().size(); i++) {
    for (int j = 0; j < getPlayers()[i].size(); j++) {
      // Skip the current player's position
      if (i == getCurrentPosition().first && j == getCurrentPosition().second) {
        continue;
      }
      if (getPlayers()[i][j] > 1) {
        return true;
      }
    }
  }
  return false;
}

std::string State::serializeState()
{
  std::string serialized_state;
  serialized_state += std::to_string(getCurrentPosition().first) + ";" + std::to_string(getCurrentPosition().second) + ";";
  serialized_state += std::to_string(getHealth()) + ";" + std::to_string(getRemainingActionCount()) + ";";
  serialized_state += std::to_string(getDamageOutput()) + ";" + std::to_string(getDamageInput()) + ";";
  serialized_state += std::to_string(getCanAttackRange()) + ";" + std::to_string(getCanAttackMelee()) + ";";
  serialized_state += std::to_string(getCanHeal()) + ";";
  serialized_state += std::to_string(getDoorPosition().first) + ";" + std::to_string(getDoorPosition().second) + ";";
  serialized_state += Utils::serializeMap(getEnemies()) + ";";
  serialized_state += Utils::serializeMap(getPlayers()) + ";";
  serialized_state += Utils::serializeMap(getLootables());
  return serialized_state;
}

void State::deserializeState(std::string state_string)
{
  // Each item is separated by a semicolon
  std::vector<std::string> state_items = Utils::splitString(state_string, ";");
  setCurrentPosition(std::make_pair(std::stoi(state_items[0]), std::stoi(state_items[1])));
  setHealth(std::stoi(state_items[2]));
  setRemainingActionCount(std::stoi(state_items[3]));
  setDamageOutput(std::stoi(state_items[4]));
  setDamageInput(std::stoi(state_items[5]));
  setCanAttackRange(std::stoi(state_items[6]));
  setCanAttackMelee(std::stoi(state_items[7]));
  setCanHeal(std::stoi(state_items[8]));
  setDoorPosition(std::stoi(state_items[9]), std::stoi(state_items[10]));
  setEnemies(Utils::deserializeMap(state_items[11]));
  setPlayer(Utils::deserializeMap(state_items[12]));
  setLootables(Utils::deserializeMap(state_items[13]));
}