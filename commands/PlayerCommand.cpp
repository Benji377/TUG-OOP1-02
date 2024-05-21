#include "PlayerCommand.hpp"
#include "../game/Game.hpp"

void PlayerCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 2);

  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);

  if(player == nullptr)
  {
    throw UnavailableItemOrEntityCommand();
  }

  player->printPlayer(game_->getCurrentRoom()->getFieldOfEntity(player), false);

}