//----------------------------------------------------------------------------------------------------------------------
//
// The Room class represents a room in the game world. It contains an id and a 2D vector of fields.
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//----------------------------------------------------------------------------------------------------------------------
//
#ifndef ROOM_HPP
#define ROOM_HPP

#include "Field.hpp"
#include <vector>
#include <memory>

using std::vector;
using std::shared_ptr;

class Room
{
  private:
    int id_;
    vector<vector<shared_ptr<Field>>> fields_;
    int enemy_count_;
    bool is_complete_;
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Private helper function to print an entity
    /// @param entity the entity to print
    //
    void printEntitie(shared_ptr<Entity> entity);
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Private helper function that prints the separation line
    //
    void printSeparationLine();

  public:
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Room class
    /// @param id the id of the room
    /// @param width the width of the room
    /// @param height the height of the room
    //
    Room(int id, int width, int height);
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the id
    /// @return the id of the room
    //
    int getId() const { return id_; }
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the fields
    /// @return the fields of the room
    //
    vector<vector<shared_ptr<Field>>> getFields() const { return fields_; }
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Sets an entity to a field
    /// @param entity the entity to set
    /// @param row the row of the field
    /// @param col the column of the field
    //
    void setFieldEntity(shared_ptr<Entity> entity, int row, int col);
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Prints the room
    //
    void printRoom();

    ///-----------------------------------------------------------------------------------------------------------------
    /// Retrieves characters present in the room along with their corresponding positions.
    ///
    /// This function iterates over the fields of the room, checks if each field contains an entity,
    /// and if so, associates the entity with its position in the room.
    ///
    /// @return A map containing pointers to the entities found in the room as keys, and their positions as values.
    ///
    std::map<std::shared_ptr<Entity>, std::string> getCharacters();


};

#endif // ROOM_HPP