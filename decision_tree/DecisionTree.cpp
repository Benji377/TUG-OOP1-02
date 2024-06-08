#include "DecisionTree.hpp"
#include <iostream>
#include <utility>

Action DecisionTree::traverse(std::shared_ptr<Player> player) const {
  std::shared_ptr<DecisionNode> current_node = root_;

  while (current_node->isConditionNode()) {
    if (current_node->evaluateCondition(game_, player)) {
      current_node = current_node->true_branch;
    } else {
      current_node = current_node->false_branch;
    }
  }

  return current_node->reviewAction(game_, player);
}

std::shared_ptr<DecisionNode> DecisionTree::createDecisionTree() {
  // Define conditions
  auto is_current_room_complete = [](Game* game, std::shared_ptr<Player> player) {
    return game->getCurrentRoom()->isComplete();
  };

  auto current_room_has_loot = [](Game* game, std::shared_ptr<Player> player) {
    return game->getCurrentRoom()->hasLoot();
  };

  auto is_loot_nearby = [](Game* game, std::shared_ptr<Player> player) {
    return game->getCurrentRoom()->isLootNearby(player);
  };

  auto is_enemy_nearby = [](Game* game, std::shared_ptr<Player> player) {
    return game->getCurrentRoom()->isEnemyNearby(player);
  };

  auto player_is_low_health = [](Game* game, std::shared_ptr<Player> player) {
    return player->getHealth() < player->getMaximumHealth() / 2;
  };

  auto player_has_weapon_equipped = [](Game* game, std::shared_ptr<Player> player) {
    return player->getWeapon() != nullptr;
  };

  auto player_has_range_weapon_equipped = [](Game* game, std::shared_ptr<Player> player) {
    return player->hasRangeWeaponEquipped();
  };

  auto player_has_ammunition_for_equipped_weapon = [](Game* game, std::shared_ptr<Player> player) {
    return player->getInventory()->hasAmmunitionForWeapon(player->getWeapon());
  };

  auto player_has_healing_potion = [](Game *game, std::shared_ptr<Player> player)
  {
    return player->hasHealingPotion();
  };

  auto is_best_range_weapon_equipped_with_ammunition = [](Game* game, std::shared_ptr<Player> player) {
    std::shared_ptr<Weapon> best_range_weapon = player->getBestRangeWeaponWithAmmunition();
    return player->getWeapon() == best_range_weapon && player->getInventory()->hasAmmunitionForWeapon(best_range_weapon);
  };

  auto player_has_range_weapon_with_ammunition = [](Game* game, std::shared_ptr<Player> player) {
    return player->hasRangeWeaponWithAmmunition();
  };

  auto player_has_melee_weapon = [](Game* game, std::shared_ptr<Player> player) {
    return player->hasMeleeWeapon();
  };

  auto player_has_melee_weapon_equipped = [](Game* game, std::shared_ptr<Player> player) {
    return player->hasMeleeWeaponEquipped();
  };

  auto path_to_enemy_exists = [](Game* game, std::shared_ptr<Player> player) {
    std::pair<int, int> next_position;
    int distance;
    return game->getCurrentRoom()->getBestMoveToEnemy(player, next_position, distance);
  };

  auto player_has_better_armor = [](Game* game, std::shared_ptr<Player> player) {
    return player->hasBetterArmor();
  };

  // Define actions
  auto move_to_enemy = [](Game* game, std::shared_ptr<Player> player) {
    std::pair<int, int> next_position;
    int distance;
    game->getCurrentRoom()->getBestMoveToEnemy(player, next_position, distance);
    return Action{MOVE, player->getAbbreviation(), next_position, distance};
  };

  auto move_to_door = [](Game* game, std::shared_ptr<Player> player) {
    std::pair<int, int> next_position;
    int distance;
    game->getCurrentRoom()->getBestMoveToDoor(player, next_position, distance);
    return Action{MOVE, player->getAbbreviation(), next_position, distance};
  };

  auto move_to_loot = [](Game* game, std::shared_ptr<Player> player) {
    std::pair<int, int> next_position;
    int distance;
    game->getCurrentRoom()->getBestMoveToLoot(player, next_position, distance);
    return Action{MOVE, player->getAbbreviation(), next_position, distance};
  };

  auto collect_loot = [](Game* game, std::shared_ptr<Player> player) {
    std::pair<int, int> loot_position = game->getCurrentRoom()->getClosestLootposition(player);
    return Action{LOOT, player->getAbbreviation(), loot_position};
  };

  auto use_health_potion = [](Game* game, std::shared_ptr<Player> player) {
    return Action{USE, player->getAbbreviation(), player->getHealingPotion()->getAbbreviation()};
  };

  auto attack_enemy_with_melee = [](Game *game, std::shared_ptr<Player> player)
  {
    int damage = player->getWeapon()->getDamage();
    std::pair<int, int> closest_enemy_position = game->getCurrentRoom()->getClosestEnemyPosition(player);
    std::vector<AttackedField> attacked_fields = game->getDungeon().simulateAttack(player, damage, closest_enemy_position);
    return Action{ATTACK, player->getAbbreviation(), closest_enemy_position, attacked_fields};
  };

  auto attack_enemy_with_range = [](Game *game, std::shared_ptr<Player> player)
  {
    int damage = player->getWeapon()->getDamage();
    std::pair<int, int> lowest_hp_enemy_position = game->getCurrentRoom()->getLowestHealthEnemyPosition();
    std::vector<AttackedField> attacked_fields = game->getDungeon().simulateAttack(player, damage, lowest_hp_enemy_position);
    return Action{ATTACK, player->getAbbreviation(), lowest_hp_enemy_position, attacked_fields};
  };

  auto equip_best_melee_weapon = [](Game* game, std::shared_ptr<Player> player) {
    std::shared_ptr<Weapon> best_melee_weapon = player->getBestMeleeWeapon();
    return Action{USE, player->getAbbreviation(), best_melee_weapon->getAbbreviation()};
  };

  auto equip_best_range_weapon_with_ammunition = [](Game* game, std::shared_ptr<Player> player) {
    return Action{USE, player->getAbbreviation(), player->getBestRangeWeaponWithAmmunition()->getAbbreviation()};
  };

  auto equip_best_armor = [](Game* game, std::shared_ptr<Player> player) {
    return Action{USE, player->getAbbreviation(), player->getBestArmor()->getAbbreviation()};
  };

  auto dummy_action = [](Game* game, std::shared_ptr<Player> player) {
    return Action{MOVE, player->getAbbreviation(), std::make_pair(0, 0)};
  };

  // Create decision tree
  auto root = std::make_shared<DecisionNode>(is_current_room_complete, "Is current room complete?");
  root->true_branch = std::make_shared<DecisionNode>(current_room_has_loot, "Does the current room contain loot?");
  root->true_branch->true_branch = std::make_shared<DecisionNode>(is_loot_nearby, "Is loot nearby?");
  root->true_branch->true_branch->true_branch = std::make_shared<DecisionNode>(collect_loot, "Collect loot");
  root->true_branch->true_branch->false_branch = std::make_shared<DecisionNode>(move_to_loot, "Move to loot");
  root->true_branch->false_branch = std::make_shared<DecisionNode>(player_is_low_health, "Is the player low on health?");
  root->true_branch->false_branch->true_branch = std::make_shared<DecisionNode>(player_has_healing_potion, "Does the player have a healing potion?");
  root->true_branch->false_branch->true_branch->true_branch = std::make_shared<DecisionNode>(use_health_potion, "Use health potion");
  root->true_branch->false_branch->true_branch->false_branch = std::make_shared<DecisionNode>(dummy_action, "TODO: Implement action");
  root->true_branch->false_branch->false_branch = std::make_shared<DecisionNode>(player_has_better_armor, "Does the player have better armor?");
  root->true_branch->false_branch->false_branch->true_branch = std::make_shared<DecisionNode>(equip_best_armor, "Equip best armor");
  root->true_branch->false_branch->false_branch->false_branch = std::make_shared<DecisionNode>(move_to_door, "Move to door");
  root->false_branch = std::make_shared<DecisionNode>(player_has_weapon_equipped, "Has the player a weapon equipped?");
  root->false_branch->true_branch = std::make_shared<DecisionNode>(is_enemy_nearby, "Is enemy nearby?");
  root->false_branch->true_branch->true_branch = std::make_shared<DecisionNode>(player_has_melee_weapon_equipped, "Has the player a melee weapon equipped?");
  root->false_branch->true_branch->true_branch->true_branch = std::make_shared<DecisionNode>(attack_enemy_with_melee, "Attack enemy");
  root->false_branch->true_branch->true_branch->false_branch = std::make_shared<DecisionNode>(equip_best_melee_weapon, "Equip best melee weapon");
  root->false_branch->true_branch->false_branch = std::make_shared<DecisionNode>(player_has_range_weapon_with_ammunition, "Has the player a range weapon with ammunition?");
  root->false_branch->true_branch->false_branch->true_branch = std::make_shared<DecisionNode>(is_best_range_weapon_equipped_with_ammunition, "Is the best range weapon with ammunition equipped?");
  root->false_branch->true_branch->false_branch->true_branch->true_branch = std::make_shared<DecisionNode>(attack_enemy_with_range, "Attack enemy with range weapon");
  root->false_branch->true_branch->false_branch->true_branch->false_branch = std::make_shared<DecisionNode>(equip_best_range_weapon_with_ammunition, "Equip best range weapon with ammunition");
  root->false_branch->true_branch->false_branch->false_branch = std::make_shared<DecisionNode>(path_to_enemy_exists, "Does a path to the enemy exist?");
  root->false_branch->true_branch->false_branch->false_branch->true_branch = std::make_shared<DecisionNode>(move_to_enemy, "Move to enemy");
  root->false_branch->true_branch->false_branch->false_branch->false_branch = std::make_shared<DecisionNode>(is_loot_nearby, "Is loot nearby?");
  root->false_branch->true_branch->false_branch->false_branch->false_branch->true_branch = std::make_shared<DecisionNode>(collect_loot, "Collect loot");
  root->false_branch->true_branch->false_branch->false_branch->false_branch->false_branch = std::make_shared<DecisionNode>(move_to_loot, "Move to loot");
  root->false_branch->false_branch = std::make_shared<DecisionNode>(is_enemy_nearby, "Is enemy nearby?");
  root->false_branch->false_branch->true_branch = std::make_shared<DecisionNode>(player_has_melee_weapon, "Has the player a melee weapon?");
  root->false_branch->false_branch->true_branch->true_branch = std::make_shared<DecisionNode>(equip_best_melee_weapon, "Equip best melee weapon");
  root->false_branch->false_branch->true_branch->false_branch = std::make_shared<DecisionNode>(player_has_range_weapon_with_ammunition, "Has the player a range weapon with ammunition?");
  root->false_branch->false_branch->true_branch->false_branch->true_branch = std::make_shared<DecisionNode>(is_best_range_weapon_equipped_with_ammunition, "Is the best range weapon with ammunition equipped?");
  root->false_branch->false_branch->true_branch->false_branch->true_branch->true_branch = std::make_shared<DecisionNode>(attack_enemy_with_range, "Attack enemy with range weapon");
  root->false_branch->false_branch->true_branch->false_branch->true_branch->false_branch = std::make_shared<DecisionNode>(equip_best_range_weapon_with_ammunition, "Equip best range weapon with ammunition");
  root->false_branch->false_branch->true_branch->false_branch->false_branch = std::make_shared<DecisionNode>(dummy_action, "TODO: Implement action");
  root->false_branch->false_branch->false_branch = std::make_shared<DecisionNode>(player_has_range_weapon_with_ammunition, "Has the player a range weapon with ammunition?");
  root->false_branch->false_branch->false_branch->true_branch = std::make_shared<DecisionNode>(is_best_range_weapon_equipped_with_ammunition, "Is the best range weapon with ammunition equipped?");
  root->false_branch->false_branch->false_branch->true_branch->true_branch = std::make_shared<DecisionNode>(attack_enemy_with_range, "Attack enemy with range weapon");
  root->false_branch->false_branch->false_branch->true_branch->false_branch = std::make_shared<DecisionNode>(equip_best_range_weapon_with_ammunition, "Equip best range weapon with ammunition");
  root->false_branch->false_branch->false_branch->false_branch = std::make_shared<DecisionNode>(player_has_melee_weapon, "Has the player a melee weapon?");
  root->false_branch->false_branch->false_branch->false_branch->true_branch = std::make_shared<DecisionNode>(equip_best_melee_weapon, "Equip best melee weapon");
  root->false_branch->false_branch->false_branch->false_branch->false_branch = std::make_shared<DecisionNode>(dummy_action, "TODO: Implement action");

  return root;
}
