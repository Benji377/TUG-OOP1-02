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
  Effect resistant_to_; // Potion effect enum
  void initializeInventory();
  public:
    Player(int id, char abbreviation); // Temporary constructor
    Player(std::string name, int health, int attack, int defense, Effect resistance);
    void attack(Character& target, int damage) override;
    void take_damage(int damage) override;
    int move(int row, int column) override;
    void printPlayer() const;
    ~Player() override = default; // Fixed error? Mogsch la ändon wennis ondos hom willsch
};


#endif //PLAYER_HPP
