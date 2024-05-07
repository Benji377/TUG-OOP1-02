#include "Dungeon.hpp"
#include "../utility/Utils.hpp"
#include "../entity/Door.hpp"
#include "../entity/character/Enemy.hpp"
#include "../entity/TreasureChest.hpp"
#include <fstream>
#include <string>
#include <iostream>

using std::string;
using std::shared_ptr;

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
          chest = std::make_shared<TreasureChest>(std::stoi(parameters[0]));
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
  action_count_ = 0;
}

void Dungeon::printCurrentRoom()
{
  current_room_->printRoom();
}