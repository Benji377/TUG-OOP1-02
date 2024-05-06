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
          rooms_[room_id]->setEntity(door, entity_row, entity_col);
          break;
        case 'Z':
        case 'G':
        case 'L':
          enemy = std::make_shared<Enemy>(std::stoi(parameters[0]), entity_abbreviation);
          rooms_[room_id]->setEntity(enemy, entity_row, entity_col);
          break;
        case 'T':
          chest = std::make_shared<TreasureChest>(std::stoi(parameters[0]));
          rooms_[room_id]->setEntity(chest, entity_row, entity_col);
          break;
        default:
          break;
        }
      }
    }

    // Temporary: Print the dungeon layout
    for (int i = 1; i < rooms_.size(); i++)
    {
      std::cout << "Room " << i << ":\n" << std::endl;
      for (int j = 0; j < rooms_[i]->getFields().size(); j++)
      {
        for (int k = 0; k < rooms_[i]->getFields()[j].size(); k++)
        {
          if (rooms_[i]->getFields()[j][k]->getEntity() != nullptr)
          {
            std::cout << " " << rooms_[i]->getFields()[j][k]->getEntity()->getAbbreviation() << " ";
          }
          else
          {
            std::cout << " - ";
          }
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }
    //

    file.close();
  }
}