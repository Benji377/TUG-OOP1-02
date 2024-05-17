//---------------------------------------------------------------------------------------------------------------------
//
// The Dice class represents an "unrolled" dice. It has a number of sides and a number of dice that can be rolled.
// The class also has a function to parse a string into a dice. The string is usually like this: "2d6" which means
// 2 dice with 6 sides each. The class also has a function to roll the dice and return the result.
//
// Group: 068
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
#ifndef DICE_HPP
#define DICE_HPP

#include <iostream>
#include "../Random.hpp"

class Dice
{
  int dice_type_;
  int roll_amount_;
  //-------------------------------------------------------------------------------------------------------------------
  ///
  /// Parses a string into a pair of integers. The string should be in the format "2d6" or "2 d6" which means
  /// 2 dice with 6 sides each.
  /// @param dice The string to be parsed.
  ///
  /// @return A pair of integers where the first is the amount of dice and the second is the dice size.
  //
  std::pair<int, int> parseDice(const std::string& dice);

  public:
    //-----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Dice class. Initializes the dice with the given dice type and roll amount.
    /// @param dice_type The number of sides of the dice.
    /// @param roll_amount The number of dice to be rolled.
    //
    Dice(int dice_type, int roll_amount);
    //-----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Dice class. Parses the given string and initializes the dice with
    /// the given dice type and roll amount.
    /// @param dice The string to be parsed.
    //
    explicit Dice(const std::string& dice);
    //-----------------------------------------------------------------------------------------------------------------
    ///
    /// Rolls the dice and returns the result. Is marked as nodiscard because the result should be used.
    /// @return The result of the dice roll.
    //
    [[nodiscard]] int roll() const;
    //-----------------------------------------------------------------------------------------------------------------
    ///
    /// Deleted copy constructor.
    //
    Dice(const Dice& other) = delete;
    //-----------------------------------------------------------------------------------------------------------------
    ///
    /// Deleted assignment operator.
    //
    Dice& operator=(const Dice& other) = delete;
    //-----------------------------------------------------------------------------------------------------------------
    ///
    /// Virtual default destructor for the Dice class.
    //
    virtual ~Dice() = default;

    //-----------------------------------------------------------------------------------------------------------------
    ///
    /// Getter functions for the dice roll amount
    /// @return The dice roll amount.
    //
    [[nodiscard]] int getAmount() const { return roll_amount_; };

    //-----------------------------------------------------------------------------------------------------------------
    ///
    /// Getter function for the dice type
    /// @return The dice type.
    //
    [[nodiscard]] int getType() const { return dice_type_; };

};


#endif //DICE_HPP
