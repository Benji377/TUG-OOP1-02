#include <iostream>

#ifndef DICE_HPP
#define DICE_HPP

/**
 * A simple class to represent a dice with a given number of sides and the amount of times it should be rolled.
 * Furthermore it also has a function to parse a string into a dice. The string is usually like this:
 * "2d6" which means 2 dice with 6 sides each.
 */
class Dice {
private:
    int dice_type_;
    int roll_amount_;
    std::pair<int, int> parseDice(const std::string& dice);
public:
  Dice(int dice_type, int roll_amount);
  explicit Dice(const std::string& dice);
  [[nodiscard]] int roll() const;
  ~Dice() = default;
  Dice(const Dice& other) = default;
};


#endif //DICE_HPP
