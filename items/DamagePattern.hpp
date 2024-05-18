//---------------------------------------------------------------------------------------------------------------------
//
// The DamagePattern class is used to determine the fields affected by a certain pattern. It is used to calculate the
// damage dealt by a weapon to a target. The class is initialized with a string that represents the pattern.
// It works by creating a field of the same size as the game field and then marking the fields that are affected by the
// pattern. The affected fields are then returned as a vector of vectors of integers. The class has methods
// for each pattern that can be used to calculate the affected fields.
//
// Group: 086
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//
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

  ///------------------------------------------------------------------------------------------------------------------
  ///
  /// Parses the pattern of the damage pattern. Converts a string to a Pattern.
  ///
  /// @param pattern The pattern of the damage pattern.
  ///
  /// @return The Pattern of the damage pattern.
  //
  Pattern parsePattern(std::string &pattern);

  public:
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// The constructor of the DamagePattern class. Initializes the pattern_ attribute with the pattern of the damage
    /// pattern. Is set to explicit to avoid implicit conversion.
    ///
    /// @param pattern The pattern of the damage pattern.
    //
    explicit DamagePattern(std::string &pattern);

    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the pattern of the damage pattern.
    ///
    /// @return The pattern of the damage pattern.
    //
    Pattern getPattern() const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the affected fields of the hit pattern. A single field is affected.
    ///
    /// @param player_pos The position of the player.
    /// @param target_field The position of the target field.
    /// @param affected_fields The fields affected by the pattern.
    ///
    /// @return The affected fields of the hit pattern.
    //
    std::vector<std::vector<int>> hitPattern(std::pair<int, int> player_pos, std::pair<int, int> target_field,
                                             std::vector<std::vector<int>> affected_fields) const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the affected fields of the slash pattern. A 3-field line or corner pattern.
    ///
    /// @param player_pos The position of the player.
    /// @param target_field The position of the target field.
    /// @param affected_fields The fields affected by the pattern.
    ///
    /// @return The affected fields of the slash pattern.
    //
    std::vector<std::vector<int>> slashPattern(std::pair<int, int> player_pos, std::pair<int, int> target_field,
                                               std::vector<std::vector<int>> affected_fields) const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the affected fields of the line pattern. Basically a straight line from the player to the target.
    ///
    /// @param player_pos The position of the player.
    /// @param target_field The position of the target field.
    /// @param affected_fields The fields affected by the pattern.
    ///
    /// @return The affected fields of the line pattern.
    //
    std::vector<std::vector<int>> linePattern(std::pair<int, int> player_pos, std::pair<int, int> target_field,
                                              std::vector<std::vector<int>> affected_fields) const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the affected fields of the burst pattern. Player can also shoot himself. A cross pattern.
    ///
    /// @param target_field The position of the target field.
    /// @param affected_fields The fields affected by the pattern.
    ///
    /// @return The affected fields of the burst pattern.
    //
    std::vector<std::vector<int>> burstPattern(std::pair<int, int> target_field,
                                               std::vector<std::vector<int>> affected_fields) const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the affected fields of the shot pattern. Player can also shoot himself. A single field is affected.
    ///
    /// @param target_field The position of the target field.
    /// @param affected_fields The fields affected by the pattern.
    ///
    /// @return The affected fields of the shot pattern.
    //
    std::vector<std::vector<int>> shotPattern(std::pair<int, int> target_field,
                                              std::vector<std::vector<int>> affected_fields) const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the affected fields of the thrust pattern. A line of two fields is affected.
    ///
    /// @param target_field The position of the target field.
    /// @param affected_fields The fields affected by the pattern.
    ///
    /// @return The affected fields of the thrust pattern.
    //
    std::vector<std::vector<int>> thrustPattern(std::pair<int, int> player_pos, std::pair<int, int> target_field,
                                                std::vector<std::vector<int>> affected_fields) const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Returns the affected fields of the pattern. The pattern is determined by the pattern_ attribute.
    ///
    /// @param player_position The position of the player.
    /// @param target_field The position of the target field.
    /// @param width The width of the game field.
    /// @param height The height of the game field.
    ///
    /// @return The affected fields of the pattern as a vector of vectors of integers. 1 = affected, 0 = not affected.
    //
    std::vector<std::vector<int>> getAffectedFields(std::pair<int, int> player_position,
                                                    std::pair<int, int> target_field, int width, int height) const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// The copy constructor of the DamagePattern class. Is deleted to avoid copying of the class.
    //
    DamagePattern(const DamagePattern &other) = delete;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// The assignment operator of the DamagePattern class. Is deleted to avoid copying of the class.
    //
    DamagePattern &operator=(const DamagePattern &other) = delete;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// The destructor of the DamagePattern class. Is defaulted.
    //
    ~DamagePattern() = default;
};

#endif //DAMAGEPATTERN_HPP
