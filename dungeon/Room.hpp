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

#include <vector>
#include <memory>
#include <cmath> //For adjacent field
#include "Field.hpp"


using std::vector;
using std::shared_ptr;

class UnavailableItemOrEntityCommand;
class Character;

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
    void printEntityInMap(shared_ptr<Entity> entity);
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
    /// Getter for the is_complete_ attribute
    ///
    /// @return A boolean value indicating whether the room is complete or not.
    ///
    bool isComplete() const { return is_complete_; }
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
    /// Retrieves enemies present in the room
    ///
    /// @return A vector containing pointers to the entities found in the room
    ///
    std::vector<std::shared_ptr<Character>> getEnemies();
    ///-----------------------------------------------------------------------------------------------------------------
    /// Retrieves the abbreviation of the enemies present in the room
    ///
    /// @return A vector containing the abbreviations of the enemies found in the room
    ///
    std::vector<char> getEnemiesAbbreviations();
    ///-----------------------------------------------------------------------------------------------------------------
    /// Returns the field at the specified position in the room.
    ///
    /// @param position A pair containing the row and column indices of the field to retrieve.
    /// @return A shared pointer to the field at the specified position in the room.
    ///
    std::shared_ptr<Field> getField(std::pair<int, int> position) { return fields_[(position.first - 1)][(position.second - 1)]; }
    ///-----------------------------------------------------------------------------------------------------------------
    /// Retrieves the field position of a given entity in the room.
    ///
    /// This function iterates over the fields of the room, checking if each field contains the specified entity.
    /// If the entity is found, the function returns a pair containing the row and column indices of the field
    /// where the entity is located. If the entity is not found, the function throws an UnavailableItemOrEntityCommand
    /// exception and returns a pair with values (-1, -1).
    ///
    /// @param entity A shared pointer to the entity to search for in the room.
    /// @return A pair containing the row and column indices of the field where the entity is located.
    ///
    /// @throws UnavailableItemOrEntityCommand if the entity is not found in any field of the room.
    ///
    std::pair<int, int> getFieldOfEntity(shared_ptr<Entity> entity);
    ///-----------------------------------------------------------------------------------------------------------------
    /// Retrieves the surrounding fields of a given field in the room.
    ///
    /// This function takes the row and column indices of a field in the room and returns a vector containing
    /// shared pointers to the fields surrounding the specified field.
    ///
    /// @param row The row index of the field for which to retrieve the surrounding fields.
    /// @param col The column index of the field for which to retrieve the surrounding fields.
    /// @return A vector containing shared pointers to the fields surrounding the specified field.
    ///
    std::vector<std::shared_ptr<Field>> getSurroundingFields(std::pair<int, int> position, int distance = 1);

    std::vector<std::pair<int, int>> getSurroundingFieldPositions(std::pair<int, int> position, int distance = 1);

    template <typename T>
    std::vector<std::shared_ptr<T>> getAllEntitiesOfType() {
      std::vector<std::shared_ptr<T>> entities;
      for (const auto& row : fields_) {
          for (const auto& field : row) {
              if (field->getEntity() != nullptr) {
                  std::shared_ptr<T> entity = std::dynamic_pointer_cast<T>(field->getEntity());
                  if (entity != nullptr) {
                      entities.push_back(entity);
                  }
              }
          }
      }
      return entities;
    }

    int getHeight() const { return fields_.size(); }
    int getWidth() const { return fields_[0].size(); }
    //Only a suggestion, you can oc change this if you want to. I originially had it in the Commands.cpp
    //but it actually belongs here, right? -Hanno
    bool isAdjacentField(std::pair<int,int> field_1, std::pair<int,int> field_2);

    //as well as this
    bool isValidField(std::pair<int,int> field);
};

#endif // ROOM_HPP