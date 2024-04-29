#include "Item.hpp"

#ifndef AMMUNITION_HPP
#define AMMUNITION_HPP

enum class AmmunitionType { ARROW, BOLT };

class Ammunition: public Item
{
  AmmunitionType type_;
  ///----------------------------------------------------------------------------------------------------------------
  ///
  /// Parses the type of the ammunition. Converts a string to an AmmunitionType.
  /// @param type The type of the ammunition.
  /// @return The AmmunitionType of the ammunition.
  //
  AmmunitionType parseType(std::string& type);
  public:
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Ammunition class.
    /// @param abbreviation The abbreviation of the ammunition.
    /// @param name The name of the ammunition.
    /// @param type The type of the ammunition.
    //
    Ammunition(std::string& abbreviation, std::string& name, std::string& type);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the type of the ammunition.
    /// @return The type of the ammunition.
    //
    [[nodiscard]] AmmunitionType getType() const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Deleted copy constructor.
    //
    Ammunition(Ammunition const&) = delete;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Deleted assignment operator.
    //
    Ammunition& operator=(Ammunition const&) = delete;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Destructor for the Ammunition class. Default implementation.
    //
    ~Ammunition() = default;
};


#endif //AMMUNITION_HPP
