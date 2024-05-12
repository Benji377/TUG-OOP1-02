//---------------------------------------------------------------------------------------------------------------------
//
// The Dungeon class represents the game world. It contains a vector of rooms, the current room, a vector of occured
// enemy types and an action count. In the constructor the dungeon layout is read from a file and the rooms are created.
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
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Dungeon class
    /// @param file_path the path to the file containing the dungeon layout
    //
    Dungeon(const char *file_path);
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to print the current room
    //
    void printCurrentRoom();

    std::shared_ptr<Room> getCurrentRoom();

    vector<char> getOccuredEnemyTypes() const { return occured_enemy_types_; }
    void addOccuredEnemyType(char type) { occured_enemy_types_.push_back(type); }
    int getCompletedRoomsCount() const;
    int getRoomCount() const { return rooms_.size() - 1; }

};

#endif // DUNGEON_HPP