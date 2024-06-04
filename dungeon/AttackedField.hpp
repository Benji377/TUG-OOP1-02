//---------------------------------------------------------------------------------------------------------------------
//
// The AttackedField class represents a field that has been attacked by a character. It contains information about the
// character that attacked the field, the amount of health the character lost, the total damage dealt, the resistance
// modifier of the character, the armor value of the character and whether the character is dead.
//
// Group: 068
//
// Author: Elija Innerkofler 12320034
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef ATTACKEDFIELD_HPP
#define ATTACKEDFIELD_HPP

#include <string>
#include <iostream>

using std::pair;
using std::string;

enum class CharacterType
{
  NONE,
  PLAYER,
  ENEMY
};

class AttackedField
{
  private:
    bool contains_character_;
    CharacterType character_type_;
    pair<int, int> position_;
    string character_name_;
    int lost_health_;
    int total_damage_;
    int resistance_modifier_;
    int armor_value_;
    bool is_dead_;

  public:
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// The constructor inizializes the AttackedField object with the given position.
    /// @param position the position of the AttackedField
    //
    AttackedField(pair<int, int> position) : contains_character_(false), position_(position), lost_health_(0),
      total_damage_(0), resistance_modifier_(0), armor_value_(0), is_dead_(false) {}
    
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the name of the character on the AttackedField
    ///
    /// @return the name of the character on the AttackedField
    //
    string getName() const { return character_name_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the position of the AttackedField
    ///
    /// @return the position of the AttackedField
    //
    pair<int, int> getPosition() const { return position_; };
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns whether the Character on the AttackedField is dead
    ///
    /// @return true if the Character is dead, false otherwise
    //
    bool isDead() const { return is_dead_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Returns whether the AttackedField contains a character
    ///
    /// @return true if the AttackedField contains a character, false otherwise
    //
    bool containsCharacter() const { return contains_character_; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Sets the name of the character on the AttackedField
    ///
    /// @param name the name of the character
    //
    void setName(string name) { character_name_ = name; }
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Sets the character on the AttackedField with the given parameters
    ///
    /// @param name the name of the character
    /// @param lost_health the health the character lost
    /// @param total_damage the total damage dealt
    /// @param resistance_modifier the resistance modifier of the character
    /// @param armor_value the armor value of the character
    /// @param is_dead whether the character is dead
    //
    void setCharacter(string name, CharacterType character_type, int lost_health, int total_damage,
      int resistance_modifier, int armor_value, bool is_dead);
    //------------------------------------------------------------------------------------------------------------------
    ///
    /// Sets the character without a name on the AttackedField with the given parameters
    ///
    /// @param lost_health the health the character lost
    /// @param total_damage the total damage dealt
    /// @param resistance_modifier the resistance modifier of the character
    /// @param armor_value the armor value of the character
    /// @param is_dead whether the character is dead
    //
    void setCharacterWithoutName(CharacterType character_type, int lost_health, int total_damage,
      int resistance_modifier, int armor_value, bool is_dead);
    //------------------------------------------------------------------------------------------------------------------

    CharacterType getChracterType() { return character_type_;};

    ///
    /// Friend function to overload the << operator for the AttackedField class
    ///
    /// @param os the output stream
    /// @param attacked_field the AttackedField that should be printed
    ///
    /// @return the output stream
    //
    friend std::ostream& operator<<(std::ostream& os, const AttackedField& attacked_field);
};

#endif // ATTACKEDFIELD_HPP