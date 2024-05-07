//---------------------------------------------------------------------------------------------------------------------
//
// The Player class is derived from the Character class, and it represents the player in the game. It extends the
// base class by adding a name and a resistance attribute. The resistance attribute is used to calculate the damage
// the player takes from an enemy attack.
//
// Group: 086
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Character.hpp"

class Player: public Character
{
  std::string name_;
  void initializeInventory();
  public:
    Player(int id, char abbreviation, std::string name);
    void attack(Character& target, int damage) override;
    void take_damage(int damage) override;
    int move(int row, int column) override;
    void setResistance(Effect effect);
    [[nodiscard]] Effect getResistance() const;
    void printPlayer(const std::pair<std::string, std::string>& position) const;
    std::string getName() const { return name_; }
    ~Player() override;
};


#endif //PLAYER_HPP
