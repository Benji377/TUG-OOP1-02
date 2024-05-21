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
#include "../entity/character/Player.hpp"
#include "../entity/character/Enemy.hpp"
#include "../dungeon/AttackedField.hpp"
#include "Room.hpp"

using std::vector;
using std::shared_ptr;

class Dungeon
{
  private:
    vector<shared_ptr<Room>> rooms_;
    shared_ptr<Room> current_room_;
    vector<char> occured_enemy_types_;
    bool boss_dead_;

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

    bool isBossDead() { return boss_dead_;};
    bool playersInRoomAreDead();
    vector<char> getOccuredEnemyTypes() const { return occured_enemy_types_; }
    void addOccuredEnemyType(char type) { occured_enemy_types_.push_back(type); }
    int getCompletedRoomsCount() const;
    int getRoomCount() const { return rooms_.size() - 1; }
    int moveToRoom(int room_id);
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Helper function for moveToRoom that removes all players from the current room and returns them
    /// @return a map containing the players and their previous positions
    //
    vector<shared_ptr<Player>> exitCurrentRoom();
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Helper function for moveToRoom that adds all players to the new room
    //
    void enterCurrentRoom(int door_id, vector<shared_ptr<Player>> players);

    void characterMove(shared_ptr<Character> character, std::pair<int, int> position);

    void moveToRandomField(std::shared_ptr<Enemy> enemy);

    vector<AttackedField> characterAttack(shared_ptr<Character> attacker, int damage, std::pair<int, int> target_field);

    void lootEntity(std::shared_ptr<Player> player, std::shared_ptr<Entity> entity);

    void killCharacter(std::shared_ptr<Character> character);
};

#endif // DUNGEON_HPP