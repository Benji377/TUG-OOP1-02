//---------------------------------------------------------------------------------------------------------------------
//
// The Ammunition class is a derived class of the Item class. It represents an ammunition item in the game.
// The class has a type attribute that represents the type of the ammunition. The type can be either ARROW or BOLT.
// The class has a constructor that initializes the attributes of the ammunition.
// The class has a parseType method that converts a string to an AmmunitionType.
//
// Group: 086
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef AMMUNITION_HPP
#define AMMUNITION_HPP

#include "Item.hpp"
#include "../utility/Utils.hpp"
#include <map>

enum class AmmunitionType
{
  ARROW, BOLT
};

class Ammunition : public Item
{
  AmmunitionType type_;
  std::vector<std::string> weapons_;
  int amount_;

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Parses the type of the ammunition. Converts a string to an AmmunitionType.
  ///
  /// @param type The type of the ammunition.
  /// @return The AmmunitionType of the ammunition.
  //
  AmmunitionType parseType(std::string &type);

public:
  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Constructor for the Ammunition class.
  ///
  /// @param abbreviation The abbreviation of the ammunition.
  /// @param name The name of the ammunition.
  /// @param type The type of the ammunition.
  //
  Ammunition(std::string &abbreviation, std::string &type, std::vector<std::string> &weapons, int amount);

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Returns the type of the ammunition.
  ///
  /// @return The type of the ammunition.
  //
  AmmunitionType getType() const;

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Sets the amount of ammunition in the stack.
  ///
  /// @param amount The amount of ammunition in the stack.
  //
  void setAmount(int amount);

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Returns the amount of ammunition in the stack.
  ///
  /// @return The amount of ammunition in the stack.
  //
  int getAmount() const;

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Deleted copy constructor.
  //
  Ammunition(Ammunition const &) = delete;

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Deleted assignment operator.
  //
  Ammunition &operator=(Ammunition const &) = delete;

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Destructor for the Ammunition class. Default implementation.
  //
  ~Ammunition() = default;
};

#endif //AMMUNITION_HPP
