#include "State.hpp"

State::State(char current_player, std::pair<int, int> current_position, int health, int remaining_enemies,
              int damage_output, int damage_input, std::map<char, int> distance_to_enemy,
              std::map<char, int> distance_to_player, std::map<char, int> distance_to_lootable,
              int distance_to_exit, int distance_to_entry, std::vector<std::vector<int>> enemies,
              std::vector<std::vector<int>> players, std::vector<std::vector<int>> lootables,
              std::pair<int, int> entry_door_position, std::pair<int, int> exit_door_position)
  : current_player_(current_player), current_position_(current_position), health_(health),
    damage_output_(damage_output), damage_input_(damage_input), remaining_enemies_(remaining_enemies),
    distance_to_enemy_(distance_to_enemy), distance_to_player_(distance_to_player),
    distance_to_lootable_(distance_to_lootable), distance_to_exit_(distance_to_exit),
    distance_to_entry_(distance_to_entry), enemies_(enemies), players_(players), lootables_(lootables),
    entry_door_position_(entry_door_position), exit_door_position_(exit_door_position)
{
  can_attack_melee_ = getCanAttackMelee();
  can_attack_range_ = getCanAttackRange();
  can_heal_ = false;
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
      if (getRemainingEnemies() == 0 || (std::make_pair(new_y, new_x) != getExitDoorPosition() &&
          std::make_pair(new_y, new_x) != getEntryDoorPosition()))
      {
        possible_moves.insert(move.first);
      }
    }
  }

  return possible_moves;
}

bool State::canLoot() {
  bool canLoot = std::any_of(distance_to_lootable_.begin(), distance_to_lootable_.end(), []
              (const std::pair<char, int>& lootable) {
    return lootable.second == 0;
  });
  return canLoot;
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
  if (getRemainingEnemies() > 0 && (canAttackAdjacent(player) || canAttackAnywhere(player))) {
    return true;
  }
  return false;
}



bool State::canAttackAdjacent(Player player)
{
  // Check if there is an enemy in any of the 8 adjacent positions, only for melee weapons
  if (player.getWeapon()->getAttackType() == AttackType::MELEE) {
    // Check if there is any enemy has the distance of 1 from the player
    bool canAttack = std::any_of(distance_to_enemy_.begin(), distance_to_enemy_.end(), []
            (const std::pair<char, int>& enemy) {
      return enemy.second == 0;
    });
    setCanAttackMelee(canAttack);
    return canAttack;
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
  // Returns true if there is at least another player in the player map
  return getDistanceToPlayer().size() > 1;
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
  serialized_state += std::to_string(getCurrentPlayer()) + "|"
          + std::to_string(getCurrentPosition().first) + "|"
          + std::to_string(getCurrentPosition().second) + "|"
          + std::to_string(getHealth())+ "|"
          + std::to_string(getDamageOutput()) + "|"
          + std::to_string(getDamageInput()) + "|"
          + std::to_string(getRemainingEnemies()) + "|"
          + Utils::serializeDistanceMap(getDistanceToEnemy()) + "|"
          + Utils::serializeDistanceMap(getDistanceToPlayer()) + "|"
          + Utils::serializeDistanceMap(getDistanceToLootable()) + "|"
          + std::to_string(getDistanceToExit()) + "|"
          + std::to_string(getDistanceToEntry()) + "|"
          + std::to_string(getCanAttackRange()) + "|"
          + std::to_string(getCanAttackMelee()) + "|"
          + std::to_string(getCanHeal()) + "|";
  return serialized_state;
}

void State::deserializeState(std::string state_string)
{
  // Each item is separated by a pipe
  std::vector<std::string> state_items = Utils::splitString(state_string, "|");
  setCurrentPlayer(state_items[0][0]); // Take the first character of the string
  setCurrentPosition(std::make_pair(std::stoi(state_items[1]), std::stoi(state_items[2])));
  setHealth(std::stoi(state_items[3]));
  setDamageOutput(std::stoi(state_items[4]));
  setDamageInput(std::stoi(state_items[5]));
  setRemainingEnemies(std::stoi(state_items[6]));
  setDistanceToEnemy(Utils::deserializeDistanceMap(state_items[7]));
  setDistanceToPlayer(Utils::deserializeDistanceMap(state_items[8]));
  setDistanceToLootable(Utils::deserializeDistanceMap(state_items[9]));
  setDistanceToExit(std::stoi(state_items[10]));
  setDistanceToEntry(std::stoi(state_items[11]));
  setCanAttackRange(std::stoi(state_items[12]));
  setCanAttackMelee(std::stoi(state_items[13]));
  setCanHeal(std::stoi(state_items[14]));
}

bool State::operator==(const State& other) const
{
  return current_player_ == other.current_player_ &&
         current_position_ == other.current_position_ &&
         // Health can vary of about 10% of the total health
         health_ >= other.health_ * 0.9 && health_ <= other.health_ * 1.1 &&
         // Damage output and damage input can vary of about +/- 2 points
         damage_output_ >= other.damage_output_ - 2 && damage_output_ <= other.damage_output_ + 2 &&
         damage_input_ >= other.damage_input_ - 2 && damage_input_ <= other.damage_input_ + 2 &&
         remaining_enemies_ == other.remaining_enemies_ &&
         distance_to_enemy_ == other.distance_to_enemy_ &&
         distance_to_player_ == other.distance_to_player_ &&
         distance_to_lootable_ == other.distance_to_lootable_ &&
         distance_to_exit_ == other.distance_to_exit_ &&
         distance_to_entry_ == other.distance_to_entry_ &&
         can_attack_melee_ == other.can_attack_melee_ &&
         can_attack_range_ == other.can_attack_range_ &&
         can_heal_ == other.can_heal_;
}

bool State::operator<(const State& other) const
{
  return serializeState() < other.serializeState();
}