#ifndef DECISION_HPP
#define DECISION_HPP

#include "../game/Game.hpp"

class Decision
{
  private:
    Game* game_;
    int attack_command_value_ = 100;
    int move_command_value_ = 50;
    int loot_command_value_ = 20;
    int use_command_value_ = 10;

    std::pair<double, std::string> getBestCommandForPlayer(std::shared_ptr<Player> player);

  public:
   Decision(Game* game) : game_(game) {}

    std::string getBestCommand();
    
    template <typename T>
    std::pair<double, std::string> getBestMove(std::shared_ptr<Player> player)
    {
      std::pair<double, std::string> best_move = std::make_pair(50, "move ");
      best_move.second += player->getName() + " ";
      std::pair<int, int> move_position = game_->getDungeon().getCurrentRoom()->getBestMove<T>(player);
      best_move.second += std::to_string(move_position.first) + " " + std::to_string(move_position.second);
      return best_move;
    }
};

#endif