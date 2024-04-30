#include "DamagePattern.hpp"

DamagePattern::DamagePattern(std::string& pattern) {
  pattern_ = parsePattern(pattern);
}

Pattern DamagePattern::parsePattern(std::string& pattern) {
  Utils::normalizeString(pattern);
  // Uses the map function to map the string to the enum
  std::map<std::string, Pattern> patternMap = {
          {"hit", Pattern::HIT},
          {"slash", Pattern::SLASH},
          {"line", Pattern::LINE},
          {"burst", Pattern::BURST},
          {"shot", Pattern::SHOT},
          {"thrust", Pattern::THRUST}
  };
  if (patternMap.count(pattern) == 0)
  {
    throw std::invalid_argument("Invalid pattern");
  }
  return patternMap[pattern];
}

Pattern DamagePattern::getPattern() const {
  return pattern_;
}