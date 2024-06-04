#include "State.hpp"

State::State(int remaining_actions, std::pair<int, int> position, Player player, std::vector<std::vector<int>> enemies,
             std::vector<std::vector<int>> players, std::vector<std::vector<int>> lootables,
             std::pair<int, int> entry_door_position, std::pair<int, int> exit_door_position)
{
  // TODO: This constructor may throw an exception if any of the arguments are invalid or null
  current_player_ = player.getAbbreviation();
  current_position_ = position;
  health_ = player.getHealth();
  remaining_action_count_ = remaining_actions;
  damage_output_ = player.getWeapon()->getDamage();
  damage_input_ = (player.getArmor() != nullptr) ? player.getArmor()->getArmorValue() : 0;
  enemies_ = enemies;
  players_ = players;
  lootables_ = lootables;
  entry_door_position_ = entry_door_position;
  exit_door_position_ = exit_door_position;
  can_attack_range_ = canAttackAnywhere(player);
  can_attack_melee_ = canAttackAdjacent(player);
  can_heal_ = canRegenerate(player);
}



// Returns the possible actions that the robot can take in the current state
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
  if (canUseArmor(player)) {
    possible_actions.insert(RobotAction::USE_ARMOR);
  }
  if (canSwitchPlayer()) {
    possible_actions.insert(RobotAction::SWITCH_PLAYER);
  }
  if(canUseArmor(player)) {
    possible_actions.insert(RobotAction::USE_ARMOR);
  }
  return possible_actions;
}

std::set<RobotAction> State::getPossibleMoves()
{
  int enemies_left = 0;
  for (const auto& row : getEnemies()) {
    for (const auto& enemy : row) {
      if (enemy > 0) {
        enemies_left++;
      }
    }
  }

  std::set<RobotAction> possible_moves;
  std::vector<std::pair<RobotAction, std::pair<int, int>>> moves = {
          {RobotAction::MOVE_UP, {-1, 0}}, //up means one less in y
          {RobotAction::MOVE_DOWN, {1, 0}},
          {RobotAction::MOVE_LEFT, {0, -1}},
          {RobotAction::MOVE_RIGHT, {0, 1}},
          {RobotAction::MOVE_UP_LEFT, {-1, -1}},
          {RobotAction::MOVE_UP_RIGHT, {-1, 1}},
          {RobotAction::MOVE_DOWN_LEFT, {1, -1}},
          {RobotAction::MOVE_DOWN_RIGHT, {1, 1}}
  };

  // In this simplified version, the possible moves and their corresponding changes in position
  // are defined in the moves vector. The code then iterates over these moves,
  // calculates the new position for each move, and checks if the move is valid
  // (i.e., within the room and not occupied by an enemy). If the move is valid, it is added to the possible_moves set.
  // This reduces the number of conditional checks and makes the code more concise and easier to understand.
  for (const auto& move : moves) {
    int new_y = getCurrentPosition().first + move.second.first;
    int new_x = getCurrentPosition().second + move.second.second;

    if (new_y >= 0 && new_y < static_cast<int>(getEnemies().size()) // P(y,x)
        && new_x >= 0 && new_x < static_cast<int>(getEnemies()[0].size())
        && getEnemies()[new_y][new_x] == 0
        && getPlayers()[new_y][new_x] == 0
        && getLootables()[new_y][new_x] == 0) {
      // Check if the robot is not moving onto the door if there are still enemies left
      if ((std::make_pair(new_y, new_x) != getExitDoorPosition() &&
          std::make_pair(new_y, new_x) != getEntryDoorPosition()) || enemies_left == 0) 
      {
        possible_moves.insert(move.first);
      }
    }
  }

  return possible_moves;
}

bool State::canLoot() {
  // Looking at the items map and the player position. If there is an item in the player position
  // or in the 8 adjacent positions, the player can loot.
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      int new_y = getCurrentPosition().first + i;
      int new_x = getCurrentPosition().second + j;
      if (new_y >= 0 && new_y < static_cast<int>(getLootables().size())
          && new_x >= 0 && new_x < static_cast<int>(getLootables()[0].size()) &&
          getLootables()[new_y][new_x] == 1) {
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
        setCanHeal(true);
        return true;
      }
    }
  }
  setCanHeal(false);
  return false;
}

std::set<RobotAction> State::getPossibleResistances(Player player)
{
  // We simply map the effect of each potion to the corresponding resistance action
  std::set<RobotAction> possible_resistances;
  std::map<Effect, RobotAction> effectToAction = {
          {Effect::FIRE, RobotAction::SET_RES_FIRE},
          {Effect::COLD, RobotAction::SET_RES_COLD},
          {Effect::FORCE, RobotAction::SET_RES_FORCE},
          {Effect::ACID, RobotAction::SET_RES_ACID}
  };
  // We iterate over the player's potions and add the corresponding resistance action to the set
  // if the potion's effect is in the effectToAction map
  for (const auto& potion : player.getInventory()->getAllPotions()) {
    if (effectToAction.count(potion->getEffect())) {
      possible_resistances.insert(effectToAction[potion->getEffect()]);
    }
  }

  return possible_resistances;
}

bool State::canAttack(Player player)
{
  // To attack, there must be at least one enemy in the room, and depending on the player's weapon,
  // the player must be able to attack adjacent enemies or enemies from a distance
  if (hasEnemies() && (canAttackAdjacent(player) || canAttackAnywhere(player))) {
    return true;
  }
  return false;
}

bool State::hasEnemies()
{
  // Check if there are any enemies left in the room
  for (const auto& row : getEnemies()) {
    if (std::any_of(row.begin(), row.end(), [](int cell) { return cell > 0; })) {
      return true;
    }
  }
  return false;
}

bool State::canAttackAdjacent(Player player)
{
  // Check if there is an enemy in any of the 8 adjacent positions, only for melee weapons
  if (player.getWeapon()->getAttackType() == AttackType::MELEE) {
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        int new_y = getCurrentPosition().first + i;
        int new_x = getCurrentPosition().second + j;
        if (new_y >= 0 && new_y < static_cast<int>(getEnemies().size())
            && new_x >= 0 && new_x < static_cast<int>(getEnemies()[0].size()) &&
            getEnemies()[new_y][new_x] > 0) {
          setCanAttackMelee(true);
          return true;
        }
      }
    }
  }
  setCanAttackMelee(false);
  return false;
}

bool State::canAttackAnywhere(Player player)
{
  // Check if the player has the required ammunition for the ranged weapon, or a weapon that doesn't require ammunition
  if (player.getWeapon()->getAttackType() == AttackType::RANGED) {
    if (player.getWeapon()->getAbbreviation().find('Q') != std::string::npos) {
      setCanAttackRange(true);
      return true;
    } else {
      std::vector<std::shared_ptr<Ammunition>> ammunition = player.getInventory()->getAllAmmunition();
      if (!ammunition.empty()) {
        for (const auto& ammo : ammunition) {
          std::vector<std::string> ammo_weapon = ammo->getWeapons();
          if (std::find(ammo_weapon.begin(), ammo_weapon.end(), player.getWeapon()->getAbbreviation()) != ammo_weapon.end()) {
            setCanAttackRange(true);
            return true;
          }
        }
      }
    }
  }
  setCanAttackRange(false);
  return false;
}

bool State::canUseRanged(Player player)
{
  // Can use ranged weapon if the player has a ranged weapon in his inventory
  if (!player.getInventory()->getAllWeapons().empty())
  {
    for (const auto& weapon : player.getInventory()->getAllWeapons())
    {
      if (weapon->getAttackType() == AttackType::RANGED && player.getWeapon()->getAbbreviation() != weapon->getAbbreviation())
      {
        return true;
      }
    }
  }
  return false;
}

bool State::canUseMelee(Player player)
{
  auto weapons = player.getInventory()->getAllWeapons();

  // Can use melee weapon if the player has a melee weapon in his inventory
  if (!weapons.empty())
  {
    for (const auto& weapon : weapons)
    {
      if (weapon->getAttackType() == AttackType::MELEE && player.getWeapon()->getAbbreviation() != weapon->getAbbreviation())
      {
        return true;
      }
    }
  }
  return false;
}

bool State::canSwitchPlayer()
{
  // The robot can switch player if there are more than one player in the game (and not all players are dead)
  for (int i = 0; i < static_cast<int>(getPlayers().size()); i++) {
    for (int j = 0; j < static_cast<int>(getPlayers()[i].size()); j++) {
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

bool State::canUseArmor(Player player)
{
  // The robot can use armor if the player has armor in his inventory, and it provides better protection than the current armor
  if (!player.getInventory()->getAllArmor().empty())
  {
    if(player.getArmor() == nullptr)
    {
      return true;
    }
    for (const auto& armor : player.getInventory()->getAllArmor())
    {
      if (armor->getArmorValue() > player.getArmor()->getArmorValue())
      {
        return true;
      }
    }
  }
  return false;
}

std::string State::serializeState() const
{
  std::string serialized_state;
  serialized_state += std::to_string(getCurrentPlayer()) + "|";
  serialized_state += std::to_string(getCurrentPosition().first) + "|" + std::to_string(getCurrentPosition().second) + "|";
  serialized_state += std::to_string(getHealth()) + "|" + std::to_string(getRemainingActionCount()) + "|";
  serialized_state += std::to_string(getDamageOutput()) + "|" + std::to_string(getDamageInput()) + "|";
  serialized_state += std::to_string(getCanAttackRange()) + "|" + std::to_string(getCanAttackMelee()) + "|";
  serialized_state += std::to_string(getCanHeal()) + "|";
  serialized_state += std::to_string(getExitDoorPosition().first) + "|" + std::to_string(getExitDoorPosition().second) + "|";
  serialized_state += std::to_string(getEntryDoorPosition().first) + "|" + std::to_string(getEntryDoorPosition().second) + "|";
  serialized_state += Utils::serializeMap(getEnemies()) + "|";
  serialized_state += Utils::serializeMap(getPlayers()) + "|";
  serialized_state += Utils::serializeMap(getLootables());
  return serialized_state;
}

void State::deserializeState(std::string state_string)
{
  // Each item is separated by a pipe
  std::vector<std::string> state_items = Utils::splitString(state_string, "|");
  setCurrentPlayer(state_items[0][0]);
  setCurrentPosition(std::make_pair(std::stoi(state_items[1]), std::stoi(state_items[1])));
  setHealth(std::stoi(state_items[3]));
  setRemainingActionCount(std::stoi(state_items[4]));
  setDamageOutput(std::stoi(state_items[5]));
  setDamageInput(std::stoi(state_items[6]));
  setCanAttackRange(std::stoi(state_items[7]));
  setCanAttackMelee(std::stoi(state_items[8]));
  setCanHeal(std::stoi(state_items[9]));
  setExitDoorPosition(std::make_pair(std::stoi(state_items[10]), std::stoi(state_items[11])));
  setEntryDoorPosition(std::make_pair(std::stoi(state_items[12]), std::stoi(state_items[13])));
  setEnemies(Utils::deserializeMap(state_items[14]));
  setPlayers(Utils::deserializeMap(state_items[15]));
  setLootables(Utils::deserializeMap(state_items[16]));
}

bool State::operator==(const State& other) const
{
  return current_player_ == other.current_player_ &&
         current_position_ == other.current_position_ &&
         health_ == other.health_ &&
         remaining_action_count_ == other.remaining_action_count_ &&
         damage_output_ == other.damage_output_ &&
         damage_input_ == other.damage_input_ &&
         enemies_ == other.enemies_ &&
         players_ == other.players_ &&
         lootables_ == other.lootables_ &&
         entry_door_position_ == other.entry_door_position_ &&
         exit_door_position_ == other.exit_door_position_ &&
         can_attack_range_ == other.can_attack_range_ &&
         can_attack_melee_ == other.can_attack_melee_ &&
         can_heal_ == other.can_heal_;
}

bool State::operator<(const State& other) const
{
  return serializeState() < other.serializeState();
}