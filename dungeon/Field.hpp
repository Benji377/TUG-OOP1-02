//----------------------------------------------------------------------------------------------------------------------
//
// The Field class represents a field in the game world. It contains an id and an entity.
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//----------------------------------------------------------------------------------------------------------------------
//
#ifndef FIELD_HPP
#define FIELD_HPP

#include "../entity/Entity.hpp"
#include <memory>

using std::shared_ptr;

class Field
{
  private:
    inline static int field_count_ = 0;
    int id_;
    shared_ptr<Entity> entity_;

  public:
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Field class
    //
    Field() : id_(field_count_++), entity_(nullptr) {}
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the id
    /// @return the id of the field
    //
    int getId() const { return id_; }
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Getter for the entity
    /// @return the entity of the field
    //
    shared_ptr<Entity> getEntity() const { return entity_; }
    ///-----------------------------------------------------------------------------------------------------------------
    ///
    /// Setter for the entity
    /// @param entity the entity to set
    //
    void setEntity(shared_ptr<Entity> entity) { entity_ = entity; }
};

#endif // FIELD_HPP