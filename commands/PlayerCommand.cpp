#include "PlayerCommand.hpp"
#include "../game/Game.hpp"

void PlayerCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 2);

  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);
  player->printPlayer(game_->getCurrentRoom()->getFieldOfEntity(player), false);

}