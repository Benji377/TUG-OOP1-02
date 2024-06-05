#ifndef DECISIONNODE_HPP
#define DECISIONNODE_HPP

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include "../game/Game.hpp"
#include "Action.hpp"

class DecisionNode {
  private:
    std::function<bool(Game* game, std::shared_ptr<Player> player)> condition;
    std::function<Action(Game* game, std::shared_ptr<Player> player)> action;
    bool is_action_node;

  public:
    DecisionNode(std::function<bool(Game* game, std::shared_ptr<Player> player)> condition);
    DecisionNode(std::function<Action(Game* game, std::shared_ptr<Player> player)> action);

    bool isConditionNode() const;
    Action reviewAction(Game* game, std::shared_ptr<Player> player) const;
    bool evaluateCondition(Game* game, std::shared_ptr<Player> player) const;

    std::shared_ptr<DecisionNode> true_branch;
    std::shared_ptr<DecisionNode> false_branch;
};

#endif // DECISIONNODE_HPP
