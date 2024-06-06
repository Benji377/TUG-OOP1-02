#include "PlayCommand.hpp"
#include "../game/Game.hpp"
#include "../decision_tree/DecisionTree.hpp"

void PlayCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);
  std::shared_ptr<DecisionTree> decision_tree = std::make_shared<DecisionTree>(game_);
  // Install Graphviz and use "dot -Tpng decision_tree.dot -o decision_tree.png" to generate a visual representation of the decision tree
  decision_tree->exportToDot("decision_tree.dot");
  for (auto player : game_->getPlayers())
  {
    Action action = decision_tree->traverse(player);
  }
}