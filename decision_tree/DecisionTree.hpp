#ifndef DECISION_TREE_HPP
#define DECISION_TREE_HPP

#include <memory>
#include <fstream>
#include "Action.hpp"
#include "../game/Game.hpp"
#include "../entity/character/Player.hpp"
#include "DecisionNode.hpp"

class DecisionTree {
private:
  Game* game_;
  std::shared_ptr<DecisionNode> root_;

public:
  DecisionTree(Game* game)
    : game_(game), root_(createDecisionTree()) {}

  Action traverse(std::shared_ptr<Player> player) const;

  std::shared_ptr<DecisionNode> createDecisionTree();

  void exportToDot(const std::string& filename) const {
    std::ofstream file(filename);
    file << "digraph DecisionTree {\n";
    int node_id = 0;
    root_->toDot(file, node_id);
    file << "}\n";
  }
};

#endif // DECISION_TREE_HPP
