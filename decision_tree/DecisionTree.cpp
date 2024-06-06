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

  auto player_has_range_weapon = [](Game* game, std::shared_ptr<Player> player) {
    return player->hasRangeWeapon();
  };

  auto player_has_range_weapon_equipped = [](Game* game, std::shared_ptr<Player> player) {
    std::cout << "Debug: player_has_range_weapon_equipped" << std::endl;
    return player->hasRangeWeaponEquipped();
  };

  auto player_has_range_weapon_ammunition = [](Game* game, std::shared_ptr<Player> player) {
    std::cout << "Debug: player_has_range_weapon_ammunition" << std::endl;
    return player->hasRangeWeaponAmmunition();
  };

  auto player_has_ammunition_for_equipped_weapon = [](Game* game, std::shared_ptr<Player> player) {
    std::cout << "Debug: player_has_ammunition_for_equipped_weapon" << std::endl;
    return player->hasAmmunitionForEquippedWeapon();
  };

  auto player_has_melee_weapon = [](Game* game, std::shared_ptr<Player> player) {
    std::cout << "Debug: player_has_melee_weapon" << std::endl;
    return player->hasMeleeWeapon();
  };

  auto player_has_melee_weapon_equipped = [](Game* game, std::shared_ptr<Player> player) {
    std::cout << "Debug: player_has_melee_weapon_equipped" << std::endl;
    return player->hasMeleeWeaponEquipped();
  };

  // Define actions
  auto move_to_enemy = [](Game* game, std::shared_ptr<Player> player) {
    std::pair<int, int> next_position;
    int distance;
    game->getCurrentRoom()->getBestMoveToEnemy(player, next_position, distance);
    std::cout << "Player " << player->getName() << " moves to " << next_position.first << ", " << next_position.second << " with distance " << distance << std::endl;
    return Action{MOVE, next_position, player->getId()};
  };

  auto move_to_door = [](Game* game, std::shared_ptr<Player> player) {
    std::cout << "Player " << player->getName() << " moves to door" << std::endl;
    return Action{MOVE, {0, 0}, player->getId()};
  };

  auto move_to_loot = [](Game* game, std::shared_ptr<Player> player) {
    std::cout << "Player " << player->getName() << " moves to loot" << std::endl;
    return Action{MOVE, {0, 0}, player->getId()};
  };

  auto collect_loot = [](Game* game, std::shared_ptr<Player> player) {
    std::pair<int, int> loot_position = game->getCurrentRoom()->getClosestLootposition(player);
    std::cout << "Player " << player->getName() << " collects loot at " << loot_position.first << ", " << loot_position.second << std::endl;
    return Action{LOOT, {0, 0}, player->getId()};
  };

  auto use_health_potion = [](Game* game, std::shared_ptr<Player> player) {
    std::cout << "Player " << player->getName() << " uses health potion" << std::endl;
    return Action{USE, {0, 0}, player->getId()};
  };

  auto attack_enemy_with_melee = [](Game* game, std::shared_ptr<Player> player) {
    std::cout << "Player " << player->getName() << " attacks enemy with melee weapon" << std::endl;
    return Action{ATTACK, {0, 0}, player->getId()};
  };

  auto attack_enemy_with_range = [](Game* game, std::shared_ptr<Player> player) {
    std::cout << "Player " << player->getName() << " attacks enemy with range weapon" << std::endl;
    return Action{ATTACK, {0, 0}, player->getId()};
  };

  auto equip_best_melee_weapon = [](Game* game, std::shared_ptr<Player> player) {
    std::cout << "Player " << player->getName() << " equips best melee weapon" << std::endl;
    return Action{USE, {0, 0}, player->getId()};
  };

  auto dummy_action = [](Game* game, std::shared_ptr<Player> player) {
    std::cout << "Player " << player->getName() << " does nothing" << std::endl;
    return Action{MOVE, {0, 0}, player->getId()};
  };

  // Create decision tree
  auto root = std::make_shared<DecisionNode>(is_current_room_complete, "Is current room complete?");
  root->true_branch = std::make_shared<DecisionNode>(current_room_has_loot, "Does current room have loot?");
  root->true_branch->true_branch = std::make_shared<DecisionNode>(is_loot_nearby, "Is loot nearby?");
  root->true_branch->true_branch->true_branch = std::make_shared<DecisionNode>(collect_loot, "Collect loot");
  root->true_branch->true_branch->false_branch = std::make_shared<DecisionNode>(move_to_loot, "Move to loot");
  root->true_branch->false_branch = std::make_shared<DecisionNode>(player_is_low_health, "Is player low health?");
  root->true_branch->false_branch->true_branch = std::make_shared<DecisionNode>(use_health_potion, "Use health potion");
  root->true_branch->false_branch->false_branch = std::make_shared<DecisionNode>(move_to_door, "Move to door");
  root->false_branch = std::make_shared<DecisionNode>(player_has_range_weapon, "Does player have range weapon?");
  root->false_branch->true_branch = std::make_shared<DecisionNode>(player_has_range_weapon_equipped, "Does player have range weapon equipped?");
  root->false_branch->true_branch->true_branch = std::make_shared<DecisionNode>(player_has_ammunition_for_equipped_weapon, "Does player have ammunition for equipped weapon?");
  root->false_branch->true_branch->true_branch->true_branch = std::make_shared<DecisionNode>(attack_enemy_with_range, "Attack enemy with range weapon");
  root->false_branch->true_branch->true_branch->false_branch = std::make_shared<DecisionNode>(is_enemy_nearby, "Is enemy nearby?");
  root->false_branch->true_branch->true_branch->false_branch->true_branch = std::make_shared<DecisionNode>(player_has_melee_weapon, "Does player have melee weapon?");
  root->false_branch->true_branch->true_branch->false_branch->true_branch->true_branch = std::make_shared<DecisionNode>(player_has_melee_weapon_equipped, "Does player have melee weapon equipped?");
  root->false_branch->true_branch->true_branch->false_branch->true_branch->true_branch->true_branch = std::make_shared<DecisionNode>(attack_enemy_with_melee, "Attack enemy with melee weapon");
  root->false_branch->true_branch->true_branch->false_branch->true_branch->true_branch->false_branch = std::make_shared<DecisionNode>(dummy_action, "TODO: Implement this branch");
  root->false_branch->true_branch->true_branch->false_branch->true_branch->false_branch = std::make_shared<DecisionNode>(equip_best_melee_weapon, "Equip best melee weapon");
  root->false_branch->true_branch->true_branch->false_branch->false_branch = std::make_shared<DecisionNode>(dummy_action, "TODO: Implement this branch");
  root->false_branch->true_branch->false_branch = std::make_shared<DecisionNode>(dummy_action, "TODO: Implement this branch");
  root->false_branch->false_branch = std::make_shared<DecisionNode>(is_enemy_nearby, "Is enemy nearby?");
  root->false_branch->false_branch->true_branch = std::make_shared<DecisionNode>(attack_enemy_with_melee, "Attack enemy with melee weapon");
  root->false_branch->false_branch->false_branch = std::make_shared<DecisionNode>(is_loot_nearby, "Is loot nearby?");
  root->false_branch->false_branch->false_branch->true_branch = std::make_shared<DecisionNode>(collect_loot, "Collect loot");
  root->false_branch->false_branch->false_branch->false_branch = std::make_shared<DecisionNode>(move_to_loot, "Move to loot");

  return root;
}
