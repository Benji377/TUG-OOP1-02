#include "Room.hpp"

Room::Room(int id, int width, int height)
{
  id_ = id;
  enemy_count_ = 0;
  is_complete_ = false;
  for (int i = 0; i < height; i++)
  {
    vector<shared_ptr<Field>> row;
    for (int j = 0; j < width; j++)
    {
      row.push_back(std::make_shared<Field>());
    }
    fields_.push_back(row);
  }
}