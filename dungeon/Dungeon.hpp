//---------------------------------------------------------------------------------------------------------------------
//
// Dungeon. Similar to Map in A1
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <vector>
#include <memory>
#include "Room.hpp"

using std::vector;
using std::shared_ptr;

class Dungeon
{
  private:
    vector<shared_ptr<Room>> rooms_;
    shared_ptr<Room> current_room_;
    vector<char> occured_enemy_types_;
    int action_count_;

  public:
    Dungeon(const char *file_path);
};

#endif // DUNGEON_HPP