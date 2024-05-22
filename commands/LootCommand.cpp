#include "LootCommand.hpp"
#include "../game/Game.hpp"
#include "../entity/TreasureChest.hpp"
#include "../entity/DeathLocation.hpp"

void LootCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 3);

  std::pair<std::shared_ptr<Player>, std::pair<int,int>> player_and_field = getPlayerAndAdjacentField(params);
  std::shared_ptr<Player> player = player_and_field.first;
  std::pair<int,int> target_position = player_and_field.second;

  std::shared_ptr<Field> field = game_->getCurrentRoom()->getField(target_position);
  std::shared_ptr<Entity> entity_on_field = field->getEntity();

  if(std::dynamic_pointer_cast<TreasureChest>(entity_on_field) != nullptr)
  {
    std::shared_ptr<TreasureChest> chest = std::dynamic_pointer_cast<TreasureChest>(entity_on_field);

    int min_value_to_roll = chest->getMinValue() - player->getVitality();

    std::cout << "** To unlock this chest you need to roll at least "
    << min_value_to_roll << " to reach the " << chest->getMinValue() << " needed.\n";

    std::shared_ptr<Dice> dice = std::make_shared<Dice>("1 d20");
    int roll_result = dice->roll();
    InputOutput::printDiceRoll(roll_result, dice);

    if(roll_result < min_value_to_roll)
    {
      std::cout << game_->getStory().getStorySegment("N_LOOT_CHEST_LOCKED");
      game_->plusOneActionCount();
      game_->printStoryAndRoom(false, true, true);
      return;
    }
  }
  else if(std::dynamic_pointer_cast<DeathLocation>(entity_on_field) == nullptr)
  {
    throw InvalidPositionCommand();
  }

  std::cout << game_->getStory().getStorySegment("N_LOOT_SUCCESSFUL");

  std::map<string, int> loot = entity_on_field->getLoot();
  std::shared_ptr<Inventory> inv_of_entity = std::make_shared<Inventory>();
  int ret = inv_of_entity->parseInventory(loot, player->getAbbreviation(),
                                          player->getStrength(), player->getVitality());
  if (ret == 1)
  {
    std::cout << "The entity contains an unknown item. The loot could not be parsed." << std::endl;
  }
  InputOutput::printInventory(inv_of_entity, nullptr);
  game_->getDungeon().lootEntity(player, entity_on_field);
  game_->plusOneActionCount();
  game_->printStoryAndRoom(false, true, true);

}