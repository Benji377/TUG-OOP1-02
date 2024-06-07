//----------------------------------------------------------------------------------------------------------------------
//
// The Dungeon class represents the game world. It contains a vector of rooms, the current room, a vector of occured
// enemy types and an action count. In the constructor the dungeon layout is read from a file and the rooms are created.
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//----------------------------------------------------------------------------------------------------------------------
//

#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <vector>
#include <memory>
#include "../entity/character/Player.hpp"
#include "../entity/character/Enemy.hpp"
#include "../dungeon/AttackedField.hpp"
#include "Room.hpp"

using std::pair;
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
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Dungeon class
    /// @param file_path the path to the file containing the dungeon layout
    //
    Dungeon(const char *file_path);

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Function to print the current room
    //
    void printCurrentRoom();
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the current room
    ///
    /// @return the current room
    //
    shared_ptr<Room> getCurrentRoom() const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the room count
    ///
    /// @return the number of rooms
    //
    int getRoomCount() const { return rooms_.size() - 1; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the number of completed rooms
    ///
    /// @return the number of completed rooms
    //
    int getCompletedRoomsCount() const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the occured enemy types
    ///
    /// @return the occured enemy types
    //
    vector<char> getOccuredEnemyTypes() const { return occured_enemy_types_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the boss_dead_ attribute
    ///
    /// @return a boolean value indicating whether the boss is dead or not
    //
    bool isBossDead() const { return boss_dead_; };
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns whether all players in the current room are dead
    ///
    /// @return a boolean value indicating whether all players in the current room are dead
    //
    bool playersInRoomAreDead() const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Adds an enemy type to the occured enemy types
    ///
    /// @param type the type of the enemy
    //
    void addOccuredEnemyType(char type) { occured_enemy_types_.push_back(type); }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Moves all players to the next room
    ///
    /// @param door_id the id of the door to move to
    ///
    /// @return 1 if the next room is room_id is the exit door, 0 otherwise
    //
    int moveToRoom(int room_id);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Helper function for moveToRoom that removes all players from the current room and returns them
    ///
    /// @return a map containing the players and their previous positions
    //
    vector<shared_ptr<Player>> exitCurrentRoom();
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Helper function for moveToRoom that adds all players to the new room
    ///
    /// @param door_id the id of the door to move to
    /// @param players the players to add
    //
    void enterCurrentRoom(int door_id, vector<shared_ptr<Player>> players);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Moves a character to a new field
    ///
    /// @param character the character to move
    /// @param position the position to move to
    //
    void characterMove(shared_ptr<Character> character, pair<int, int> position);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Moves an enemy to a random field
    ///
    /// @param enemy the enemy to move
    //
    void moveToRandomField(shared_ptr<Enemy> enemy);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// This function handles the attack of a character
    ///
    /// @param attacker the character attacking
    /// @param damage the damage dealt
    /// @param target_field the field to attack
    ///
    /// @return a vector of AttackedFields containing the information of the attack
    //
    vector<AttackedField> characterAttack(shared_ptr<Character> attacker, int damage, pair<int, int> target_field);

    vector<AttackedField> simulateAttack(shared_ptr<Character> attacker, int damage, pair<int, int> target_field);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// This function handles the loot of an entity
    ///
    /// @param player the player looting
    /// @param entity the entity to loot
    //
    void lootEntity(shared_ptr<Player> player, shared_ptr<Entity> entity);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// This function kills a character
    ///
    /// @param character the character to kill
    //
    void killCharacter(shared_ptr<Character> character);
};

#endif // DUNGEON_HPP