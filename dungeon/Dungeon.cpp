#include "Dungeon.hpp"
#include "../utility/Utils.hpp"
#include "../entity/Door.hpp"
#include "../entity/character/Enemy.hpp"
#include "../entity/TreasureChest.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

using std::map;
using std::shared_ptr;
using std::string;

Dungeon::Dungeon(const char *file_path)
{
  std::ifstream file(file_path);
  if (file.is_open())
  {
    string line;
    std::getline(file, line);
    rooms_.push_back(nullptr); // Add a dummy room at index 0
    while (std::getline(file, line))
    {
      // Split the line by room and entities
      vector<string> entities = Utils::splitString(line, ";");
      string temp_room_info = entities[0];
      entities.erase(entities.begin());
      // Create a room object
      vector<string> room_info = Utils::splitString(temp_room_info, ":");
      int room_id = std::stoi(room_info[0]);
      vector<string> room_dimensions = Utils::splitString(room_info[1], ",");
      int room_width = std::stoi(room_dimensions[0]);
      int room_height = std::stoi(room_dimensions[1]);
      rooms_.push_back(std::make_shared<Room>(room_id, room_width, room_height));
      if (room_id == 1)
      {
        current_room_ = rooms_[room_id];
      }
      // Add entities to the room
      for (string entity : entities)
      {
        if (entity.empty())
        {
          continue;
        }
        vector<string> entity_info = Utils::splitString(entity, ":");
        char entity_abbreviation = entity_info[0][0];
        vector<string> entity_position = Utils::splitString(entity_info[1], ",");
        int entity_row = std::stoi(entity_position[0]);
        int entity_col = std::stoi(entity_position[1]);
        vector<string> parameters = Utils::splitString(entity_info[2], ",");
        // TEMPORARY: Create entities based on their abbreviation (TODO: Implement factory pattern)
        shared_ptr<Door> door;
        shared_ptr<Enemy> enemy;
        shared_ptr<TreasureChest> chest;
        map<string, int> treasure_chest_loot;
        switch (entity_abbreviation)
        {
        case 'D':
          door = std::make_shared<Door>(std::stoi(parameters[0]));
          rooms_[room_id]->setFieldEntity(door, entity_row, entity_col);
          break;
        case 'Z':
        case 'G':
        case 'L':
          enemy = std::make_shared<Enemy>(std::stoi(parameters[0]), entity_abbreviation);
          rooms_[room_id]->setFieldEntity(enemy, entity_row, entity_col);
          break;
        case 'T':
          for (size_t i = 1; i < parameters.size(); i+=2)
          {
            treasure_chest_loot[parameters[i]] = std::stoi(parameters[i+1]);
          }
          chest = std::make_shared<TreasureChest>(std::stoi(parameters[0]), treasure_chest_loot);
          rooms_[room_id]->setFieldEntity(chest, entity_row, entity_col);
          break;
        default:
          break;
        }
      }
    }
    file.close();
  }
  occured_enemy_types_ = {};
}

void Dungeon::printCurrentRoom()
{
  current_room_->printRoom();
}

std::shared_ptr<Room> Dungeon::getCurrentRoom()
{
  return current_room_;
}

int Dungeon::getCompletedRoomsCount() const
{
  int count = 0;
  for (auto room : rooms_)
  {
    if (room != nullptr)
    {
      if (room->isComplete())
      {
        count++;
      }
    }
  }
  return count;
}

void Dungeon::moveToRoom(int room_id)
{
  int current_room_id = current_room_->getId();
  vector<shared_ptr<Player>> players = exitCurrentRoom();
  current_room_ = rooms_[room_id];
  enterCurrentRoom(current_room_id, players);
  printCurrentRoom();
}

vector<shared_ptr<Player>> Dungeon::exitCurrentRoom()
{
  vector<vector<shared_ptr<Field>>> fields = current_room_->getFields();
  vector<shared_ptr<Player>> players;
  for (size_t i = 0; i < fields.size(); i++)
  {
    for (size_t j = 0; j < fields[i].size(); j++)
    {
      if (fields[i][j]->getEntity() != nullptr)
      {
        shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(fields[i][j]->getEntity());
        if (player != nullptr)
        {
          current_room_->setFieldEntity(nullptr, i, j);
          players.push_back(player);
        }
      }
    }
  }
  sort(players.begin(), players.end(), [](shared_ptr<Player> a, shared_ptr<Player> b) ->
    bool { return a->getId() < b->getId(); });
  return players;
}

void Dungeon::enterCurrentRoom(int door_id, vector<shared_ptr<Player>> players)
{
  vector<vector<shared_ptr<Field>>> fields = current_room_->getFields();
  vector<shared_ptr<Door>> door_fields = current_room_->getAllEntitiesOfType<Door>();
  for (auto door : door_fields)
  {
    if (door->getId() == door_id)
    {
      std::pair<int, int> door_position = current_room_->getFieldOfEntity(door);
      int i = 1;
      int player_count = 0;
      while (player_count < players.size())
      {
        vector<shared_ptr<Field>> surrounding_fields = current_room_->getSurroundingFields(door_position, i);
        for (size_t j = 0; j < surrounding_fields.size(); j++)
        {
          if (surrounding_fields[j]->getEntity() == nullptr)
          {
            surrounding_fields[j]->setEntity(players[player_count]);
            player_count++;
            if ((size_t)player_count == players.size())
            {
              break;
            }
          }
        }
        i++;
      }
    }
  }
}

int Dungeon::moveCharacter(shared_ptr<Character> character, std::pair<int, int> position)
{
  std::pair<int, int> character_position = current_room_->getFieldOfEntity(character);
  current_room_->setFieldEntity(nullptr, character_position.first, character_position.second);
  current_room_->setFieldEntity(character, position.first, position.second);
  return 0;
}