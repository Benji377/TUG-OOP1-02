#include "Command.hpp"
#include "../game/Game.hpp"

void Command::checkCommandLenght(std::vector<std::string> params, size_t required_size) const
{
  if(params.size() != required_size)
  {
    throw WrongNumberOfParametersException();
  }
}

std::shared_ptr<Player> Command::getPlayerOfAbbrev(std::vector<std::string> params,
                size_t position_of_abbrev_in_params) const
{
  std::string input_abbreviation = params.at(position_of_abbrev_in_params);

  std::vector<std::shared_ptr<Player>> players = game_->getPlayers();
  char uppercase_input_abbrev = static_cast<char>(toupper(input_abbreviation[0]));

  if(input_abbreviation.length() != 1)
  {
    throw InvalidParamCommand();
  }
  if(uppercase_input_abbrev != 'W' && uppercase_input_abbrev != 'B' && uppercase_input_abbrev != 'R')
  {
    throw InvalidParamCommand();
  }

  for(auto& player : players)
  {
    char current_player_abbrev = player->getAbbreviation();

    if(current_player_abbrev == uppercase_input_abbrev && !player->isDead())
    {
      return player;
    }
  }

  return nullptr;
}

std::vector<int> Command::getPositionAsVecOutOfString(std::string position_string) const
{
  std::vector<int> position; //Works with a vector to check for stuff like 1,2,3

  try
  {
    std::vector<std::string> split_s = Utils::splitString(position_string, ",");
    if(split_s.size() != 2)
    {
      throw InvalidParamCommand();
    }
    for(const std::string& s : split_s)
    {
      size_t pos;
      int num = std::stoi(s, &pos); // pos Tells us how many digits the number stoi has found has
      if(pos < s.size()) // If this number is smaller than the size of the string, then the string is not a complete number
      {
        throw InvalidParamCommand();
      }
      // Else if pos == s.size() then the entire string is a number
      position.push_back(num);
    }
  }
  catch(const std::exception& e)
  {
    throw InvalidParamCommand();
  }
  catch(...)
  {
    throw InvalidParamCommand(); // Catch all other exceptions and convert to custom exception
  }

  if(position.size() != 2 )
  {
    throw InvalidParamCommand();
  }

  return position;
}

std::pair<int,int> Command::getPositionAsPairOutOfString(std::string position) const
{
  std::vector<int> position_as_vector = getPositionAsVecOutOfString(position);
  std::pair<int, int> position_pair = std::make_pair(position_as_vector.at(0), position_as_vector.at(1));

  return position_pair;
}

std::pair<std::shared_ptr<Player>, std::pair<int, int>>
        Command::getPlayerAndAdjacentField(std::vector<std::string> params)
{
  std::shared_ptr<Player> player = getPlayerOfAbbrev(params, 1); //Get player first to envoke right order of exceptions

  std::pair<int, int> target_position = getPositionAsPairOutOfString(params.at(2));

  if(player == nullptr)
  {
    throw UnavailableItemOrEntityCommand();
  }

  std::pair<int,int> current_position = game_->getCurrentRoom()->getFieldOfEntity(player);

  //if it's not an adjacent field
  if(!(game_->getCurrentRoom()->isAdjacentField(current_position, target_position)))
  {
    throw InvalidPositionCommand();
  }

  return std::make_pair(player, target_position);
}



void Command::updateState(shared_ptr<State> state)
{
  auto current_room = game_->getCurrentRoom();
  std::shared_ptr<Player> current_player = game_->getActivePlayerQLearn();

  auto current_pos = current_room->getFieldOfEntity(current_player);
  std::pair<int, int> new_pos = std::make_pair(current_pos.first - 1, current_pos.second - 1);

  int damage = current_player->getWeapon()->getDamageAddition() + current_player->getWeapon()->getDice()->getType();

  int armor_value = current_player->getArmor() ? current_player->getArmor()->getArmorValue() : current_player->getBaseArmor();

  bool can_heal = false;
  std::vector<std::shared_ptr<Potion>> potions = current_player->getInventory()->getAllPotions();
  for(auto potion : potions)
  {
    if(potion->getEffect() == Effect::HEALTH)
    {
      can_heal = true;
      break;
    }
  }

  std::pair<int,int> entry_door_position = current_room->getEntryDoorPosition();
  --entry_door_position.first; //Convert it to vector coordinates.
  --entry_door_position.second;

  std::pair<int,int> exit_door_position = current_room->getNextDoorPosition();
  --exit_door_position.first;
  --exit_door_position.second;

  state->updateState(current_player->getAbbreviation(), new_pos, current_player->getHealth(),
                     damage, armor_value, current_room->getCharacterAsInt<Enemy>(),
                             current_room->getCharacterAsInt<Player>(), current_room->getLootableAsInt(),
                             entry_door_position, exit_door_position, can_heal);
}