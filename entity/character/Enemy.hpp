//---------------------------------------------------------------------------------------------------------------------
//
// The enemy class is derived from the Character class, and it represents an enemy in the game. It does change much
// from the Character class, but it is a separate class to make the code more readable and to make it easier to
// implement new features in the future. Its only notable difference is we check if its a boss.
//
// Group: 086
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Character.hpp"

class Enemy: public Character
{
  bool is_boss_;
  void initializeInventory();
  public:
    Enemy(int id, char abbreviation); // Temporary constructor
    [[nodiscard]] bool is_boss() const;
    void attack(Character& target, int damage) override;
    void takeDamage(int damage) override;
    int move(int row, int column) override;
    void printEnemy() const;
    ~Enemy() = default;
    bool isEnemy() const override{ return true; }
};


#endif //ENEMY_HPP
