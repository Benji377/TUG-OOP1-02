//----------------------------------------------------------------------------------------------------------------------
//
// LOrem ipsum
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
    Field() : id_(field_count_++), entity_(nullptr) {}

    int getId() const { return id_; }
    shared_ptr<Entity> getEntity() const { return entity_; }
    void setEntity(shared_ptr<Entity> entity) { entity_ = entity; }
};

#endif // FIELD_HPP