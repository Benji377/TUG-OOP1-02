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
#include <cmath>
#include <algorithm>
#include "Field.hpp"
#include "../entity/character/Character.hpp"
#include "../entity/character/Player.hpp"
#include "../entity/Door.hpp"

using std::vector;
using std::shared_ptr;
using std::pair;

class Room
{
  private:
    int id_;
    vector<vector<shared_ptr<Field>>> fields_;
    bool is_complete_;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Private helper function to print an entity
    ///
    /// @param entity the entity to print
    //
    void printEntityInMap(shared_ptr<Entity> entity) const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Private helper function that prints the separation line
    //
    void printSeparationLine() const;

  public:
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Room class
    /// @param id the id of the room
    /// @param width the width of the room
    /// @param height the height of the room
    //
    Room(int id, int width, int height);

    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the id
    ///
    /// @return the id of the room
    //
    int getId() const { return id_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the height of the room
    ///
    /// @return the height of the room
    //
    int getHeight() const { return fields_.size(); }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the width of the room
    ///
    /// @return the width of the room
    //
    int getWidth() const { return fields_[0].size(); }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the is_complete_ attribute
    ///
    /// @return A boolean value indicating whether the room is complete or not.
    //
    bool isComplete() const { return is_complete_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the fields
    /// @return the fields of the room
    //
    vector<vector<shared_ptr<Field>>> getFields() const { return fields_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Retrieves enemies present in the room
    ///
    /// @return A vector containing pointers to the entities found in the room
    //
    vector<shared_ptr<Character>> getEnemies() const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Retrieves the abbreviation of the enemies present in the room
    ///
    /// @return A vector containing the abbreviations of the enemies found in the room
    //
    vector<char> getEnemiesAbbreviations() const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the field at the specified position in the room.
    ///
    /// @param position A pair containing the row and column indices of the field to retrieve.
    ///
    /// @return A shared pointer to the field at the specified position in the room.
    //
    shared_ptr<Field> getField(pair<int, int> position) const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Retrieves the field position of a given entity in the room.
    ///
    /// @param entity A shared pointer to the entity to search for in the room.
    ///
    /// @return A pair containing the row and column indices of the field where the entity is located.
    ///
    /// @throws UnavailableItemOrEntityCommand if the entity is not found in any field of the room.
    //
    pair<int, int> getFieldOfEntity(shared_ptr<Entity> entity) const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Retrieves the surrounding field positions of a given field in the room.
    ///
    /// @param position A pair containing the row and column indices of the field for which to retrieve the surrounding
    /// field positions.
    ///
    /// @return A vector containing pairs of row and column indices of the surrounding fields of the given field.
    //
    vector<pair<int, int>> getSurroundingFieldPositions(pair<int, int> position) const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Retrieves the empty surrounding field positions of a given field in the room. The number of the empty
    /// surrounding fields can be specified.
    ///
    /// @param position A pair containing the row and column indices of the field for which to retrieve the empty
    /// surrounding field positions.
    /// @param character_count The number of empty surrounding fields to retrieve.
    ///
    /// @return A vector containing pairs of row and column indices of the empty surrounding fields of the given field.
    //
    vector<pair<int, int>> getEmptySurroundingFieldPositions(pair<int, int> position,
      int character_count = 1) const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Retruns all entities of a given type in the room
    ///
    /// @tparam T The type of the entities to retrieve
    ///
    /// @return A vector containing shared pointers to the entities of the given type found in the room
    //
    template <typename T>
    vector<shared_ptr<T>> getAllEntitiesOfType() const {
      vector<shared_ptr<T>> entities;
      for (const auto& row : fields_) {
          for (const auto& field : row) {
              if (field->getEntity() != nullptr) {
                  shared_ptr<T> entity = std::dynamic_pointer_cast<T>(field->getEntity());
                  if (entity != nullptr) {
                      entities.push_back(entity);
                  }
              }
          }
      }
      return entities;
    }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns a 2D vector of the health of the characters in the room
    ///
    /// @tparam T The type of the characters to retrieve the health of
    ///
    /// @return A 2D vector containing the health of the characters in the room
    //
    template <typename T>
    std::vector<std::vector<int>> getCharacterAsInt()
    {
      static_assert(std::is_base_of<Character, T>::value, "T must be a subclass of Character");
      std::vector<std::vector<int>> fields_as_ints;
      std::transform(fields_.begin(), fields_.end(), std::back_inserter(fields_as_ints), [](const std::vector<std::shared_ptr<Field>>& row) {
        std::vector<int> row_as_ints;
        std::transform(row.begin(), row.end(), std::back_inserter(row_as_ints), [](const std::shared_ptr<Field>& field) {
          auto entity = field->getEntity();
          auto casted_entity = std::dynamic_pointer_cast<T>(entity);
          if (casted_entity)
          {
            return casted_entity->getHealth();
          } else
          {
            return 0;
          }
        });
        return row_as_ints;
      });
      return fields_as_ints;
    }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns a 2D vector of the lootables in the room
    ///
    /// @return A 2D vector containing the lootables in the room
    //
    vector<vector<int>> getLootableAsInt() const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the best move for a player in the room
    ///
    /// @tparam T The type of the player to get the best move for
    ///
    /// @param player the player to get the best move for
    ///
    /// @return A pair containing the row and column indices of the best move
    //
    template <typename T>
    pair<int,int> getBestMove(shared_ptr<Player> player) const
    {
      vector<pair<int, int>> surrounding_fields = getSurroundingFieldPositions(getFieldOfEntity(player));
      vector<shared_ptr<T>> available_entities = getAllEntitiesOfType<T>();
      map<int, pair<int, int>> distances;
      for (auto& field : surrounding_fields)
      {
        if (getField(field)->getEntity() == nullptr)
        {
          for (auto& entity : available_entities)
          {
            distances[getDistance(field, getFieldOfEntity(dynamic_pointer_cast<Entity>(entity)))] = field;
          }
        }
      }
      pair<int, int> best_move = distances.begin()->second;
      return best_move;
    }

    int getDistance(pair<int, int> field_1, pair<int, int> field_2) const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns whether two fields are adjacent to each other
    ///
    /// @param field_1 the first field
    /// @param field_2 the second field
    ///
    /// @return true if the fields are adjacent, false otherwise
    //
    bool isAdjacentField(pair<int,int> field_1, pair<int,int> field_2) const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns whether a field is valid. A field is valid if it is within the bounds of the room.
    ///
    /// @param field the field to check
    ///
    /// @return true if the field is valid, false otherwise
    //
    bool isValidField(pair<int,int> field) const;
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Sets the completion state of the room
    ///
    /// @param completion_state the completion state of the room
    //
    void setComplete(bool completion_state) {is_complete_ = completion_state; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Sets the entity of a field in the room
    ///
    /// @param entity the entity to set
    /// @param row the row of the field
    /// @param col the column of the field
    //
    void setFieldEntity(shared_ptr<Entity> entity, int row, int col);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Checks whether the room is complete and sets the completion state accordingly
    //
    void checkCompletion();
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Opens all doors in the room
    //
    void openDoors();
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Prints the room
    //
    void printRoom();
};

#endif // ROOM_HPP