#include "InventoryCommand.hpp"
#include "../game/Game.hpp"
#include "../utility/IO.hpp"

void InventoryCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 2);

  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1);

  if(player == nullptr)
  {
    throw UnavailableItemOrEntityCommand();
  }

  IO::printPlayerInventory(player);
}