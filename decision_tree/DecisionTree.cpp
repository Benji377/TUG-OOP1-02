#include "DecisionTree.hpp"

Action DecisionTree::traverse(std::shared_ptr<Player> player) const {
  std::shared_ptr<DecisionNode> currentNode = root_;

  while (currentNode->isConditionNode()) {
    if (currentNode->evaluateCondition(game_, player)) {
      currentNode = currentNode->true_branch;
    } else {
      currentNode = currentNode->false_branch;
    }
  }

  return currentNode->reviewAction(game_, player);
}

std::shared_ptr<DecisionNode> DecisionTree::createDecisionTree() {
  // Define conditions
  auto isCurrentRoomComplete = [](Game* game, std::shared_ptr<Player> player)
  {
    return game->getCurrentRoom()->isComplete();
  };

  auto currentRoomHasLoot = [](Game* game, std::shared_ptr<Player> player)
  {
    return game->getCurrentRoom()->hasLoot();
  };

  auto isLootNearby = [](Game* game, std::shared_ptr<Player> player)
  {
    return game->getCurrentRoom()->isLootNearby(player);
  };

  auto isEnemyNearby = [](Game *game, std::shared_ptr<Player> player)
  {
    return game->getCurrentRoom()->isEnemyNearby(player);
  };

  auto playerIsLowHealth = [](Game* game, std::shared_ptr<Player> player)
  {
    return player->getHealth() < player->getMaximumHealth() / 2;
  };

  // TODO: The find path function gets called two times, which is not efficient
  auto pathToEnemyExists = [](Game* game, std::shared_ptr<Player> player)
  {
    pair<int, int> next_position;
    int distance;
    return game->getCurrentRoom()->getBestMoveToEnemy(player, next_position, distance);
  };

  // Define actions
  auto moveToEnemy = [](Game *game, std::shared_ptr<Player> player)
  {
    pair<int, int> next_position;
    int distance;
    game->getCurrentRoom()->getBestMoveToEnemy(player, next_position, distance);
    std::cout << "Player " << player->getName() << " moves to " << next_position.first << ", " << next_position.second <<
      " with distance " << distance << std::endl;
    return Action{MOVE, next_position, player->getId()};
  };

  auto moveToDoor = [](Game* game, std::shared_ptr<Player> player)
  {
    pair<int, int> next_position;
    int distance;
    game->getCurrentRoom()->getBestMoveToDoor(player, next_position, distance);
    std::cout << "Player " << player->getName() << " moves to " << next_position.first << ", " << next_position.second <<
      " with distance " << distance << std::endl;
    return Action{MOVE, {0, 0}, player->getId()};
  };

  auto moveToLoot = [](Game* game, std::shared_ptr<Player> player)
  {
    pair<int, int> next_position;
    int distance;
    game->getCurrentRoom()->getBestMoveToLoot(player, next_position, distance);
    std::cout << "Player " << player->getName() << " moves to " << next_position.first << ", " << next_position.second <<
      " with distance " << distance << std::endl;
    return Action{MOVE, {0, 0}, player->getId()};
  };

  auto collectLoot = [](Game* game, std::shared_ptr<Player> player)
  {
    pair<int, int> loot_position = game->getCurrentRoom()->getClosestLootposition(player);
    std::cout << "Player " << player->getName() << " collects loot at " << loot_position.first << ", " << loot_position.second << std::endl;
    return Action{LOOT, {0, 0}, player->getId()};
  };

  auto useHealthPotion = [](Game* game, std::shared_ptr<Player> player)
  {
    std::cout << "Debugging: useHealthPotion" << std::endl;
    return Action{USE, {0, 0}, player->getId()};
  };

  auto attackEnemy = [](Game* game, std::shared_ptr<Player> player)
  {
    std::cout << "Debugging: attackEnemy" << std::endl;
    return Action{ATTACK, {0, 0}, player->getId()};
  };

  auto moveToNextRoom = [](Game* game, std::shared_ptr<Player> player)
  {
    std::cout << "Debugging: enterRoom" << std::endl;
    return Action{MOVE, {0, 0}, player->getId()};
  };
  
  // Create decision tree
  auto root = std::make_shared<DecisionNode>(isCurrentRoomComplete);
  root->true_branch = std::make_shared<DecisionNode>(currentRoomHasLoot);
  root->true_branch->true_branch = std::make_shared<DecisionNode>(isLootNearby);
  root->true_branch->true_branch->true_branch = std::make_shared<DecisionNode>(collectLoot);
  root->true_branch->true_branch->false_branch = std::make_shared<DecisionNode>(moveToLoot);
  root->true_branch->false_branch = std::make_shared<DecisionNode>(playerIsLowHealth);
  root->true_branch->false_branch->true_branch = std::make_shared<DecisionNode>(useHealthPotion);
  root->true_branch->false_branch->false_branch = std::make_shared<DecisionNode>(moveToDoor);
  root->false_branch = std::make_shared<DecisionNode>(isEnemyNearby);
  root->false_branch->true_branch = std::make_shared<DecisionNode>(attackEnemy);
  root->false_branch->false_branch = std::make_shared<DecisionNode>(pathToEnemyExists);
  root->false_branch->false_branch->true_branch = std::make_shared<DecisionNode>(moveToEnemy);
  root->false_branch->false_branch->false_branch = std::make_shared<DecisionNode>(isLootNearby);
  root->false_branch->false_branch->false_branch->true_branch = std::make_shared<DecisionNode>(collectLoot);
  root->false_branch->false_branch->false_branch->false_branch = std::make_shared<DecisionNode>(moveToLoot);


  return root;
}
