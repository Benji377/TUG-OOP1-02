#ifndef DECISIONTREE_HPP
#define DECISIONTREE_HPP

#include "DecisionNode.hpp"

class DecisionTree {
  private:
    Game* game_;
    std::shared_ptr<DecisionNode> root_;

    std::shared_ptr<DecisionNode> createDecisionTree();

  public:
    DecisionTree(Game* game) : game_(game), root_(createDecisionTree()) {}

    Action traverse(std::shared_ptr<Player> player) const;
};

#endif // DECISIONTREE_HPP
