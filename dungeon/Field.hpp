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

class Field
{
  private:
    inline static int field_count_ = 0;
    int id_;
    char symbol_;
    // TODO: Entity* entity_;

  public:
    Field() : id_(field_count_++), symbol_(' ') {}

    int getId() const { return id_; }
};

#endif // FIELD_HPP