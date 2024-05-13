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
#include <cstring> //for strlen in printPlayer

class Player: public Character
{
  std::string name_;
  void initializeInventory();
  public:
    Player(int id, char abbreviation, std::string name);
    void attack(Character& target, int damage) override;
    void takeDamage(int damage, DamageType damage_type) override;
    int move(int row, int column) override;
    void setResistance(DamageType damage_type);
    [[nodiscard]] DamageType getResistance() const;
    int usePotion(std::string abbreviation);
    void printPlayer(const std::pair<int, int>& position, bool single_line = false) const;
    [[nodiscard]] std::string getName() const { return name_; }
    void simplePrint() const override;

    void setActiveWeapon(std::string weapon_abbreviation);
    void setArmor(std::string armor_abbreviation);
    std::shared_ptr<Weapon> getActiveWeapon() const;


    ~Player() = default;
};


#endif //PLAYER_HPP
