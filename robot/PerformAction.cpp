#include "PerformAction.hpp"
#include "../game/Game.hpp"


double PerformAction::perform_move(Player player, std::pair<int, int> player_position, RobotAction action,
                                   std::pair<int, int> entry_door_position, std::pair<int,int> exit_door_position, int enemies_left)
{
  //Player position written in: y-Axis, x-Axis
  std::map<RobotAction, std::pair<int, int>> actionToMove = {
          {RobotAction::MOVE_UP, {-1, 0}}, //up means one less in y
          {RobotAction::MOVE_DOWN, {1, 0}},
          {RobotAction::MOVE_LEFT, {0, -1}},
          {RobotAction::MOVE_RIGHT, {0, 1}},
          {RobotAction::MOVE_UP_LEFT, {-1, -1}},
          {RobotAction::MOVE_UP_RIGHT, {-1, 1}},
          {RobotAction::MOVE_DOWN_LEFT, {1, -1}},
          {RobotAction::MOVE_DOWN_RIGHT, {1, 1}}
  };

  if (actionToMove.count(action))
  {
    int new_y = player_position.first + actionToMove[action].first;
    int new_x = player_position.second + actionToMove[action].second; 

    //But player input is in new_y-Axis and new_x-Axis
    std::string command = "move " + std::string(1, player.getAbbreviation()) + " " 
            + std::to_string(new_y + 1) + "," + std::to_string(new_x + 1) + "\n";
    std::cout << command;
    game_->doCommand(command);
    // Check if the robot moved to the exit door and there are no enemies left
    if (new_y == exit_door_position.first && new_x == exit_door_position.second && enemies_left == 0)
    {
      return REWARD_MOVE_DOOR_EXIT;
    }
    // Check if the robot moved to the entry door
    if (new_y == entry_door_position.first && new_x == entry_door_position.second)
    {
      return REWARD_MOVE_DOOR_ENTRY;
    }
    return REWARD_MOVE;
  }
  // If the action is not a move action, return a big negative number
  return REWARD_EXCEPTION;
}

double PerformAction::perform_loot(Player player, std::pair<int, int> player_position, std::vector<std::vector<int>> lootables)
{
  // Define the offsets for the surrounding cells
  std::vector<std::pair<int, int>> offsets = {{0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  for (const auto& offset : offsets) {
    int new_y = player_position.first + offset.first;
    int new_x = player_position.second + offset.second;

    // Check if the new position is within the bounds of the lootables vector
    if (new_x >= 0 && new_x < static_cast<int>(lootables.size()) && new_y >= 0 && new_y < static_cast<int>(lootables[0].size())) {
      if (lootables[new_y][new_x] == 1) {

        std::string command = "loot " + std::string(1, player.getAbbreviation()) + " " 
                + std::to_string(new_y + 1) + "," + std::to_string(new_x + 1) + "\n";
        std::cout << command;
        game_->doCommand(command);

        std::cout << "Robot looted at: (" << new_x << ", " << new_y << ")" << std::endl;
        return REWARD_LOOT;
      }
    }
  }

//This is just so we have some fail output when it finds no loot. This code can be removed later
//BEGIN
    std::string command = "loot " + std::string(1, player.getAbbreviation()) + " " 
            + std::to_string(player_position.first +1) + "," + std::to_string(player_position.second+1) + "\n";
    std::cout << command;
    game_->doCommand(command); 
    std::cout << "Robot loot failed\n"; 
//END

  // If there is no loot, return a big negative number
  return REWARD_EXCEPTION;
}

double PerformAction::perform_regeneration(Player player)
{
    std::string command = "use " + std::string(1, player.getAbbreviation()) + " ";

    std::shared_ptr<Potion> potion1 = player.getInventory()->getPotion("NHEP");
    std::shared_ptr<Potion> potion2 = player.getInventory()->getPotion("GHEP");
    std::shared_ptr<Potion> potion3 = player.getInventory()->getPotion("SHEP");
    if (potion1) {
        command += potion1->getAbbreviation();
        std::cout << command << std::endl;
        game_->doCommand(command);
    } 
    else if (potion2) {
        command += potion2->getAbbreviation();
        std::cout << command << std::endl;
        game_->doCommand(command);
    } 
    else if (potion3) {
        command += potion3->getAbbreviation();
        std::cout << command << std::endl;
        game_->doCommand(command);
    } 
    else {
        std::cout << "No suitable potion found in inventory." << std::endl;
        return REWARD_EXCEPTION; // Regeneration not possible.
    }

  // Provide reward depending on health
  if (player.getHealth() == player.getMaximumHealth())
  {
    // No health loss, health potion is wasted
    return REWARD_HEAL_FULL;
  }
  else if (player.getHealth() < player.getMaximumHealth() * 0.5) {
    // If the player has lost more than 50% of its health, provide a big reward
    return REWARD_HEAL_HALF;
  }
  else
  {
    // If the player has lost less than 50% of its health, provide a small reward
    return REWARD_HEAL_SOME;
  }
}

double PerformAction::perform_resistance(Player player, RobotAction action)
{
  // Define the resistance types
  std::map<RobotAction, std::string> resistanceTypes = {
          {RobotAction::SET_RES_FIRE, "Fire"},
          {RobotAction::SET_RES_COLD, "Cold"},
          {RobotAction::SET_RES_FORCE, "Force"},
          {RobotAction::SET_RES_ACID, "Acid"}
  };

  if (resistanceTypes.count(action))
  {
    std::string resistanceType = resistanceTypes[action];
    std::string abbreviation = resistanceType.substr(0, 2); //only first two characters
    std::transform(abbreviation.begin(), abbreviation.end(), abbreviation.begin(), ::toupper);
    abbreviation = abbreviation + "RS";

    std::string command = "use " + std::string(1, player.getAbbreviation()) + " " + abbreviation;
    std::cout << command << std::endl;
    game_->doCommand(command);

    // If the player has lost more than 10% of its health, provide a big reward
    if (player.getHealth() < player.getMaximumHealth() * 0.9)
    {
      std::cout << "Robot used " << resistanceTypes[action] << " resistance potion" << std::endl;
      return REWARD_USE_RESISTANCE_SOME;
    }
    std::cout << "Robot used " << resistanceTypes[action] << " resistance potion" << std::endl;
    return REWARD_USE_RESISTANCE_FULL;
  }
  // If the action is not a resistance action, return a big negative number
  return REWARD_EXCEPTION;
}

double PerformAction::perform_attack(Player player, std::pair<int, int> player_position, std::vector<std::vector<int>> enemies)
{
  // TODO: Add a big reward if the enemy is killed
  if (player.getWeapon()->getAttackType() == AttackType::MELEE)
  {
    std::cout << "[DEBUG] Robot has a melee weapon in attack perform" << std::endl;
    // Define the offsets for the surrounding cells
    std::vector<std::pair<int, int>> offsets = {{0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1},
                                                {-1, 1}, {1, -1}, {1, 1}};

    for (const auto &offset: offsets)
    {
      int new_y = player_position.first + offset.first;
      int new_x = player_position.second + offset.second;
      std::cout << "[DEBUG] Robot is trying to attack enemy at: (" << new_y + 1 << ", " << new_x + 1 << ")" << std::endl;

      // Check if the new position is within the bounds of the enemies vector
      if (new_y >= 0 && new_y < static_cast<int>(enemies.size()) && new_x >= 0 && new_x < static_cast<int>(enemies[0].size()))
      {
        std::cout << "[DEBUG] Field is not out of bounds" << std::endl;
        int temp = enemies[new_y][new_x]; //TODO sometimes skips over this
        std::cout << "[DEBUG] Enemy at: (" << new_y + 1 << ", " << new_x + 1 << ") has health: " << temp << std::endl;
        if (temp > 0)
        {
          std::string command = "attack " + std::string(1, player.getAbbreviation()) + " " 
                  + std::to_string(new_y + 1) + "," + std::to_string(new_x + 1) + "\n";
          std::cout << command;
          game_->doCommand(command); 
          std::cout << "Robot attacked enemy at: (" << new_x << ", " << new_y << ") using melee weapon" << std::endl;
          return REWARD_ATTACK;
        }
      }
      else
      {
        std::cout << "[DEBUG] Field is out of bounds" << std::endl;

      }
    }
  } else 
  {
    // If the player has a ranged weapon, check if there is an enemy in range
    std::cout << "[DEBUG] Robot has a ranged weapon in attack perform" << std::endl;
    for (int i = 0; i < static_cast<int>(enemies.size()); i++)
    {
      std::cout << "[DEBUG] Checking column: " << i << " of: " << enemies.size() << std::endl;
      for (int j = 0; j < static_cast<int>(enemies[0].size()); j++)
      {
        std::cout << "[DEBUG] Checking row: " << j << " of: " << enemies[0].size() << std::endl;
        if (enemies[i][j] > 0)
        {
          std::cout << "[DEBUG] Robot is trying to attack enemy at: (" << j << ", " << i << ")" << std::endl;
          std::string command = "attack " + std::string(1, player.getAbbreviation()) + " " 
                  + std::to_string(i + 1) + "," + std::to_string(j + 1) + "\n";
          std::cout << command;
          game_->doCommand(command);
          std::cout << "Robot attacked enemy at: (" << i + 1 << ", " << j + 1 << ") using ranged weapon" << std::endl;
          return REWARD_ATTACK;
        }
        else
        {
          std::cout << "[DEBUG] No enemy found at: (" << i + 1 << ", " << j + 1 << ")" << std::endl;
        }
      }
    }
  }

  // If there is no enemy to attack, return a big negative number
  return REWARD_EXCEPTION;
}

double PerformAction::perform_use_ranged(Player player)
{
  auto weapons = player.getInventory()->getAllWeapons();

  auto it = std::remove_if(weapons.begin(), weapons.end(),
                          [player](const std::shared_ptr<Weapon>& weapon) {
                              return weapon == player.getWeapon();
                          });
  weapons.erase(it, weapons.end()); //We don't want to just unequip the current weapon

    std::vector<std::shared_ptr<Weapon>> rangedWeapons; //ChatGPT Begin
    std::copy_if(weapons.begin(), weapons.end(), std::back_inserter(rangedWeapons),
              [](const std::shared_ptr<Weapon>& weapon) {
                  return weapon->getAttackType() == AttackType::RANGED;
              });
    // We use the damage addition in addition to the dice type to determine the best weapon
    // This is because a higher dice type means a higher chance of dealing more damage
    auto maxWeaponIt = std::max_element(rangedWeapons.begin(), rangedWeapons.end(),
              [](const std::shared_ptr<Weapon>& a, const std::shared_ptr<Weapon>& b) {
                  return (a->getDice()->getType() + a->getDamageAddition()) <
                        (b->getDice()->getType() + b->getDamageAddition());
              });
//ChatGPT End
  if(maxWeaponIt != rangedWeapons.end())
  {
    std::string command = "use " + std::string(1, player.getAbbreviation()) + " " 
                  + (*maxWeaponIt)->getAbbreviation() + "\n";
    std::cout << command;
    game_->doCommand(command);

    std::cout << "Robot equipped ranged weapon" << std::endl;
    return REWARD_EQUIP_WEAPON;
  }
  else
  {
    std::cout << "Robot failed to equip ranged weapon" << std::endl;
    return REWARD_EXCEPTION; //tried to equip meele but there wasn't one.
  }
}

double PerformAction::perform_use_melee(Player player)
{
  auto weapons = player.getInventory()->getAllWeapons();

  auto it = std::remove_if(weapons.begin(), weapons.end(),
                          [player](const std::shared_ptr<Weapon>& weapon) {
                              return weapon->getAbbreviation() == player.getWeapon()->getAbbreviation();
                          });
  weapons.erase(it, weapons.end()); //We don't want to just unequip the current weapon or equip the same.

    std::vector<std::shared_ptr<Weapon>> meleeWeapons;
    std::copy_if(weapons.begin(), weapons.end(), std::back_inserter(meleeWeapons),
              [](const std::shared_ptr<Weapon>& weapon) {
                  return weapon->getAttackType() == AttackType::MELEE;
              });

  auto maxWeaponIt = std::max_element(meleeWeapons.begin(), meleeWeapons.end(),
            [](const std::shared_ptr<Weapon>& a, const std::shared_ptr<Weapon>& b) {
                return (a->getDice()->getType() + a->getDamageAddition()) <
                        (b->getDice()->getType() + b->getDamageAddition());
            });

  if(maxWeaponIt != meleeWeapons.end())
  {
    std::string command = "use " + std::string(1, player.getAbbreviation()) + " " 
                  + (*maxWeaponIt)->getAbbreviation() + "\n";
    std::cout << command;
    game_->doCommand(command);

    std::cout << "Robot equipped melee weapon" << std::endl;
    return REWARD_EQUIP_WEAPON;
  }
  else
  {
    std::cout << "Robot failed to equip melee weapon" << std::endl;
    return REWARD_EXCEPTION; //tried to equip meele but there wasn't one.
  }
}

double PerformAction::perform_switch_player(char current_player, std::vector<Player> players)
{
  // Find the index of the current player
  int current_player_index = -1;
  std::cout << "[DEBUG] Performing switch player for: " << current_player << std::endl;
  // We need to iterate through the players and find the next player that is still alive
  // If there is no more player alive, we will not switch the player
  for (int i = 0; i < static_cast<int>(players.size()); i++)
  {
    if (players[i].getAbbreviation() == current_player)
    {
      current_player_index = i;
      break;
    }
  }
  for (int i = current_player_index; i < static_cast<int>(players.size()); i++)
  {
    if (i == static_cast<int>(players.size()) - 1)
    {
      i = 0;
    }
    if (!players[i].isDead())
    {
      if (i == current_player_index) {
        // In this case, no switching is needed, as its the only player left
        return REWARD_EXCEPTION;
      }
      current_player_index = i;
      break;
    }
  }


  if (current_player_index != -1)
  {
    // Switch to the next player in the list
    std::cout << "[DEBUG] Switching to next player" << std::endl;
    int next_player_index = (current_player_index + 1) % players.size();
    std::cout << "[DEBUG] Next player index: " << next_player_index << std::endl;
    std::string command = "switch " + std::string(1, game_->getPlayers().at(next_player_index)->getAbbreviation());
    std::cout << "[DEBUG] Command: " << command << std::endl;
    game_->doCommand(command);
    std::cout << "Switched to player: " << players[next_player_index].getAbbreviation() << std::endl;
    return REWARD_SWITCH_PLAYER;
  }

  // If the player is not found, return a big negative number
  return REWARD_EXCEPTION;
}

double PerformAction::perform_use_armor(Player player)
{
  auto player_armor = player.getArmor();
  // Get the best armor from the player's inventory
  if (!player.getInventory()->getAllArmor().empty()) {
    for (const auto &armor : player.getInventory()->getAllArmor()) 
    {
      if(player_armor != nullptr)
      {
        if (armor->getArmorValue() > player_armor->getArmorValue()) 
        {
        std::string command = "use " + std::string(1, player.getAbbreviation()) + " " +
                  armor->getAbbreviation() + "\n";
        std::cout << command;
        game_->doCommand(command);

        std::cout << "Robot equipped armor" << std::endl;
        return REWARD_EQUIP_ARMOR;
        }
      }
      else
      {
        std::string command = "use " + std::string(1, player.getAbbreviation()) + " " +
          armor->getAbbreviation() + "\n";
        std::cout << command;
        game_->doCommand(command);

        std::cout << "Robot equipped armor" << std::endl;
        return REWARD_EQUIP_ARMOR;
      }
    }
  }
          std::cout << "Robot failed to equip armor" << std::endl;

  return REWARD_EXCEPTION;
}
