#include "PlayCommand.hpp"
#include "../game/Game.hpp"
#include "../decision_tree/DecisionTree.hpp"

void PlayCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);
  std::shared_ptr<DecisionTree> decision_tree = std::make_shared<DecisionTree>(game_);
  for (auto player : game_->getPlayers())
  {
    Action action = decision_tree->traverse(player);
  }
}