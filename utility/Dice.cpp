#include "Dice.hpp"
#include "../Random.cpp"

Dice::Dice(int dice_type, int roll_amount) : dice_type_(dice_type), roll_amount_(roll_amount) {}

Dice::Dice(const std::string& dice)
{
  std::pair<int, int> parsed = parseDice(dice);
  roll_amount_ = parsed.first;
  dice_type_ = parsed.second;
}

int Dice::roll() const
{
  int result = 0;
  for (int i = 0; i < roll_amount_; i++) {
    result += (int)Random::getInstance().getRandomNumber(dice_type_);
  }
  return result;
}

std::pair<int, int> Dice::parseDice(const std::string& dice)
{
  size_t d_pos = dice.find('d');
  if (d_pos == std::string::npos)
  {
    // TODO: Change to custom exception
    throw std::invalid_argument("Invalid dice string");
  }
  std::string amount = dice.substr(0, d_pos);
  std::string type = dice.substr(d_pos + 1);
  int roll_amount = std::stoi(amount);
  int dice_type = std::stoi(type);
  return std::make_pair(roll_amount, dice_type);
}