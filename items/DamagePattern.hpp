// TODO: To be defined

#ifndef DAMAGEPATTERN_HPP
#define DAMAGEPATTERN_HPP

#include <map>
#include <string>
#include "Item.hpp"
#include "../utility/Utils.hpp"


enum class Pattern
{
  HIT, SLASH, THRUST, SHOT, LINE, BURST
};

class DamagePattern
{
  Pattern pattern_;
  Pattern parsePattern(std::string &pattern);

public:
  explicit DamagePattern(std::string &pattern);
  [[nodiscard]] Pattern getPattern() const;
  DamagePattern(const DamagePattern &other) = delete;
  DamagePattern &operator=(const DamagePattern &other) = delete;
  ~DamagePattern() = default;
};

#endif //DAMAGEPATTERN_HPP
