//---------------------------------------------------------------------------------------------------------------------
//
// The class Potion is a derived class of the class Item. It represents a potion in the game. A potion can either be
// a health potion or a resistance potion. Health potions restore health and are consumed on use. Resistance potions
// increase the resistance of the player and are active until the room is changed.
//
// Group: 068
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef POTION_HPP
#define POTION_HPP

#include "Item.hpp"
#include "../utility/Utils.hpp"
#include <map>


enum class Effect
{
  HEALTH, FIRE, COLD, FORCE, ACID
};

class Potion : public Item
{
  Effect effect_;

  ///-------------------------------------------------------------------------------------------------------------------
  ///
  /// Parses the effect of the potion. Converts a string to an effect.
  /// @param effect The effect of the potion as a string.
  /// @return The effect of the potion.
  //
  Effect parseEffect(std::string &effect);

public:
  ///----------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor for a potion.
  /// @param abbreviation The abbreviation of the potion.
  /// @param name The name of the potion.
  /// @param dice The dice of the potion. can be null on resistance potions.
  /// @param effect The effect of the potion.
  //
  Potion(std::string &abbreviation, std::string &name, std::string &effect, Dice *dice);

  ///----------------------------------------------------------------------------------------------------------------
  ///
  /// Getter for the effect of the potion.
  ///
  /// @return The effect of the potion.
  //
  [[nodiscard]] Effect getEffect() const;

  ///----------------------------------------------------------------------------------------------------------------
  ///
  /// Deleted copy constructor.
  //
  Potion(const Potion &) = delete;

  ///----------------------------------------------------------------------------------------------------------------
  ///
  /// Deleted assignment operator.
  //
  Potion &operator=(const Potion &) = delete;

  ///----------------------------------------------------------------------------------------------------------------
  ///
  /// Destructor for a potion. Default.
  //
  virtual ~Potion();
};

#endif //POTION_HPP
