#include "DecisionNode.hpp"

DecisionNode::DecisionNode(std::function<bool(Game* game, std::shared_ptr<Player> player)> condition)
  : condition(condition), is_action_node(false) {}

DecisionNode::DecisionNode(std::function<Action(Game* game, std::shared_ptr<Player> player)> action)
  : action(action), is_action_node(true) {}

bool DecisionNode::isConditionNode() const {
  return !is_action_node;
}

Action DecisionNode::reviewAction(Game* game, std::shared_ptr<Player> player) const {
  return action(game, player);
}

bool DecisionNode::evaluateCondition(Game* game, std::shared_ptr<Player> player) const {
  if (condition) {
    return condition(game, player);
  }
  return false;
}
