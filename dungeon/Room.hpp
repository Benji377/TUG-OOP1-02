//----------------------------------------------------------------------------------------------------------------------
//
// LOrem ipsum
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

  public:
    Room(int id, int width, int height);

    int getId() const { return id_; }
    vector<vector<shared_ptr<Field>>> getFields() const { return fields_; }
    void setEntity(shared_ptr<Entity> entity, int row, int col);

};

#endif // ROOM_HPP