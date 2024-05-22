#include "Dice.hpp"

Dice::Dice(int dice_type, int roll_amount) : dice_type_(dice_type), roll_amount_(roll_amount), previous_roll_(-1) {}

Dice::Dice(const std::string &dice)
{
  std::pair<int, int> parsed = parseDice(dice);
  roll_amount_ = parsed.first;
  dice_type_ = parsed.second;
  previous_roll_ = -1; // Will be set to a valid value after the first roll
}

int Dice::roll()
{
  int result = 0;
  for (int current_roll = 0; current_roll < getAmount(); current_roll++)
  {
    // The random number is generated in the range [1, dice_type_]
    result += static_cast<int>(Oop::Random::getInstance().getRandomNumber(getType()));
  }
  setPreviousRoll(result);
  return result;
}

std::pair<int, int> Dice::parseDice(const std::string &dice)
{
  // The accepted format is "2d6" or "2 d6" which means 2 dice with 6 sides each.
  size_t d_pos = dice.find('d');
  if (d_pos == std::string::npos)
  {
    throw std::invalid_argument("[DICE] Invalid dice string");
  }
  std::string amount = dice.substr(0, d_pos);
  std::string type = dice.substr(d_pos + 1);
  int roll_amount = std::stoi(amount);
  int dice_type = std::stoi(type);
  return std::make_pair(roll_amount, dice_type);
}