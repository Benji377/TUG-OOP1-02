#include "PositionsCommand.hpp"
#include "../game/Game.hpp"
#include "../utility/IO.hpp"

void PositionsCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 1);

  std::shared_ptr<Room> current_room = game_->getCurrentRoom();

  //Assuming Players are always stored by id, otherwise sort them
  std::vector<std::shared_ptr<Player>> players = game_->getPlayers();

  for(auto& player : players)
  {
    if(player->isDead() == false)
    {
      player->printPlayer(current_room->getFieldOfEntity(player), true);
    }
  }

  std::vector<std::shared_ptr<Character>> enemies = current_room->getEnemies();

  std::map<std::string, std::shared_ptr<Character>> enemies_mapped;

  for(auto& enemy : enemies)
  {
    std::string enemy_string_to_be_sorted = enemy->getTypeName() + " " + std::to_string(enemy->getId());
    enemies_mapped.insert(std::make_pair(enemy_string_to_be_sorted, enemy));
  }

  IO::printEnemyPosition(enemies_mapped, current_room);
}