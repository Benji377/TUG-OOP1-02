// TODO: To be defined

#ifndef DAMAGEPATTERN_HPP
#define DAMAGEPATTERN_HPP

#include <map>
#include <string>
#include <vector>
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

  std::vector<std::vector<int>> hitPattern(std::pair<int, int> player_pos, std::pair<int, int> target_field,
                                           std::vector<std::vector<int>> affected_fields) const;
  std::vector<std::vector<int>> slashPattern(std::pair<int, int> player_pos, std::pair<int, int> target_field,
                                             std::vector<std::vector<int>> affected_fields) const;
  std::vector<std::vector<int>> linePattern(std::pair<int, int> player_pos, std::pair<int, int> target_field,
                                            std::vector<std::vector<int>> affected_fields) const;
  std::vector<std::vector<int>> burstPattern(std::pair<int, int> target_field,
                                             std::vector<std::vector<int>> affected_fields) const;
  std::vector<std::vector<int>> shotPattern(std::pair<int, int> target_field,
                                            std::vector<std::vector<int>> affected_fields) const;
  std::vector<std::vector<int>> thrustPattern(std::pair<int, int> player_pos, std::pair<int, int> target_field,
                                              std::vector<std::vector<int>> affected_fields) const;

  std::vector<std::vector<int>> getAffectedFields(std::pair<int, int> player_position,
                                                  std::pair<int, int> target_field, int width, int height) const;

  void printDamagePattern(const std::vector<std::vector<int>>& damagePattern) const;

  DamagePattern(const DamagePattern &other) = delete;
  DamagePattern &operator=(const DamagePattern &other) = delete;
  ~DamagePattern() = default;
};

#endif //DAMAGEPATTERN_HPP
