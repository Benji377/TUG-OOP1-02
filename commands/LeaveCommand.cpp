#include "LeaveCommand.hpp"
#include "../entity/character/Player.hpp"
#include "../game/Game.hpp"


void LeaveCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 2);

  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);

  if(player == nullptr)
  {
    throw UnavailableItemOrEntityCommand();
  }

  player->simplePrintNoId();
  std::cout << " left the game.\n";

  game_->getDungeon().killCharacter(player);
  player->setHasQuit(true);
}