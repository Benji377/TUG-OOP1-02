#include "MoveCommand.hpp"
#include "../game/Game.hpp"
#include "../entity/Door.hpp"
#include "../utility/InputOutput.hpp"

void MoveCommand::execute(std::vector<std::string> params)
{
  checkCommandLenght(params, 3);

  std::pair<std::shared_ptr<Player>, std::pair<int,int>> player_and_field = getPlayerAndAdjacentField(params);
  std::shared_ptr<Player> player = player_and_field.first;
  std::pair<int,int> target_position = player_and_field.second;

  std::shared_ptr<Field> field = game_->getCurrentRoom()->getField(target_position);
  std::shared_ptr<Entity> entity_on_field = field->getEntity();

  if(entity_on_field == nullptr)
  {
    InputOutput::printPlayerMoved(player, target_position);
    game_->getDungeon().characterMove(player, target_position);
    game_->plusOneActionCount();
    game_->printStoryAndRoom(false, true, true);
  }
  else if(std::dynamic_pointer_cast<Door>(entity_on_field) != nullptr)
  {
    std::shared_ptr<Door> door = std::dynamic_pointer_cast<Door>(entity_on_field);
    if(door->isLocked())
    {
      throw CommandExecutionException(CommandExecutionException::ExceptionType::LOCKED_DOOR);
    }
    else
    {
      game_->plusOneActionCount();
      game_->setActionCount(0); //Action count gets reset if in a new room
      InputOutput::printPlayerMoved(player, target_position);
      int result = game_->getDungeon().moveToRoom(door->getLeadsTo());
      if (result == 1)
      {
        game_->setAdditionalreward(-500); //if they flee
        std::cout << game_->getStory().getStorySegment("N_FLIGHT");
        game_->toggleGameRunning();
        game_->printAndSaveScore();
        return;
      }
      game_->printStoryAndRoom(true , true, true);
    }

  }
  else
  {
    throw InvalidPositionCommand();
  }

}