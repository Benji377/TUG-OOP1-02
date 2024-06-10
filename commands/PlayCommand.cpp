#include "PlayCommand.hpp"
#include "../game/Game.hpp"
#include "../decision_tree/DecisionTree.hpp"
#include "../entity/TreasureChest.hpp"

const int ATTACK_BONUS = 100;
const int MOVE_BONUS = 20;
const int LOOT_BONUS = 30;
const int USE_BONUS = 50;

const int KILL_ALLY_PENALTY = -50;
const int KILL_ENEMY_BONUS = 50;

void PlayCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);
  std::shared_ptr<DecisionTree> decision_tree = std::make_shared<DecisionTree>(game_);
  // Install Graphviz and use "dot -Tpng decision_tree.dot -o decision_tree.png" to generate a visual representation of the decision tree
  decision_tree->exportToDot("decision_tree.dot");
  std::vector<Action> actions;
  for (auto player : game_->getLivingPlayers())
  {
    Action action = decision_tree->traverse(player);
    actions.push_back(action);
  }
  Action best_action = getBestAction(actions);
  std::cout << "AI: " << best_action.getCommand() << std::endl;
  game_->doCommand(best_action.getCommand());
}

Action PlayCommand::getBestAction(std::vector<Action>& actions)
{
  Action best_action = actions[0];
  for (auto& action : actions)
  {
    switch (action.getType())
    {
      case ActionType::ATTACK:
        action.setScore(ATTACK_BONUS);
        for (auto attacked_field : action.getAttackedFields())
        {
          if (attacked_field.containsCharacter())
          {
            if (attacked_field.getCharacterType() == CharacterType::PLAYER)
            {
              if (attacked_field.isDead())
              {
                action.addScore(KILL_ALLY_PENALTY);
              }
              else
              {
                action.setScore(attacked_field.getLostHealth() * -1);
              }
            }
            else
            {
              action.addScore(attacked_field.getLostHealth());
              if (attacked_field.isDead())
              {
                action.addScore(KILL_ENEMY_BONUS);
              }
            }
          }
        }
        break;
      case ActionType::MOVE:
        if(action.getDistance() == 999999999)
        {
          action.setScore(-1);
          break; //It's a dummy action and shouldn't be executed.
        }
        else if(game_->getCurrentRoom()->isComplete() && game_->isLowestPlayer(getPlayerOfAbbrev(action.getPlayerAbbreviation())))
        {
          if(action.isTargetADoor() == false)
          {
            auto player = getPlayerOfAbbrev(action.getPlayerAbbreviation());
            int multiplier = player->getMaximumHealth() - player->getHealth() + 2; //It could be that several players at once have the same hp. It then takes the one with the higest diff from max health. If all are full it just keeps it to 2
            action.setScore(MOVE_BONUS * multiplier - action.getDistance()); //so it overrides other possible loots and moves.
            break;
          }

        }
        action.setScore(MOVE_BONUS - action.getDistance());
        break;
      case ActionType::LOOT:
        action.setScore(LOOT_BONUS);
        break;
      case ActionType::USE:
        action.setScore(USE_BONUS);
        break;
    }
  }
  for (auto action : actions)
  {
    if (action.getScore() > best_action.getScore())
    {
      best_action = action;
    }
  }
  return best_action;
}