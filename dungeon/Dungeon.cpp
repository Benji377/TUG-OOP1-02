#include "Dungeon.hpp"
#include "../utility/Utils.hpp"
#include <fstream>
#include <string>
#include <iostream>

using std::string;

Dungeon::Dungeon(const char *file_path)
{
  std::ifstream file(file_path);
  if (file.is_open())
  {
    string line;
    std::getline(file, line);
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
        vector<string> entity_info = Utils::splitString(entity, ":");
        char entity_abbreviation = entity_info[0][0];
        vector<string> entity_position = Utils::splitString(entity_info[1], ",");
        int entity_row = std::stoi(entity_position[0]);
        int entity_col = std::stoi(entity_position[1]);
        vector<string> parameters = Utils::splitString(entity_info[2], ",");


      }
    }
    file.close();
  }
}