//----------------------------------------------------------------------------------------------------------------------
//
// LOrem ipsum
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//----------------------------------------------------------------------------------------------------------------------
//
#ifndef DEATHLOCATION_HPP
#define DEATHLOCATION_HPP

#include "Entity.hpp"

class DeathLocation : public Entity
{
  private:
    inline static int death_location_count_ = 0;

  public:
    DeathLocation() : Entity(death_location_count_++, 'X', true) {}
    int getId() const { return id_; }
};

#endif //DEATHLOCATION_HPP