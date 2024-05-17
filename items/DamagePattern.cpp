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
    throw std::invalid_argument("[DAMAGEPATTERN] Invalid pattern during parsing");
  }
  return patternMap[pattern];
}

Pattern DamagePattern::getPattern() const {
  return pattern_;
}

std::vector<std::vector<int>> DamagePattern::getAffectedFields(std::pair<int, int> player_position,
    std::pair<int, int> target_field, int width, int height) const
{
  // We need to normalize the player position and target field from 1-based to 0-based
  player_position.first -= 1;
  player_position.second -= 1;
  target_field.first -= 1;
  target_field.second -= 1;

  std::vector<std::vector<int>> affected_fields(height, std::vector<int>(width, 0));
  affected_fields[player_position.first][player_position.second] = 2;

  switch (getPattern())
  {
    case Pattern::HIT:
      return hitPattern(player_position, target_field, affected_fields);
    case Pattern::SLASH:
      return slashPattern(player_position, target_field, affected_fields);
    case Pattern::LINE:
      return linePattern(player_position, target_field, affected_fields);
    case Pattern::BURST:
      return burstPattern(target_field, affected_fields);
    case Pattern::SHOT:
      return shotPattern(target_field, affected_fields);
    case Pattern::THRUST:
      return thrustPattern(player_position, target_field, affected_fields);
    default:
      throw std::invalid_argument("[DAMAGEPATTERN] Invalid pattern for affected fields");
  }
}

std::vector<std::vector<int>> DamagePattern::hitPattern(std::pair<int, int> player_pos, std::pair<int, int> target_field,
                                                        std::vector<std::vector<int>> affected_fields) const
{
  if (abs ((int)(target_field.first - player_pos.first)) > 1 ||
      abs((int)(target_field.second - player_pos.second)) > 1 ||
      (target_field.first == player_pos.first && target_field.second == player_pos.second))
  {
    throw std::invalid_argument("[DAMAGEPATTERN] Hit - Invalid target field");
  }
  affected_fields[target_field.first][target_field.second] = 1;
  return affected_fields;
}

std::vector<std::vector<int>> DamagePattern::thrustPattern(std::pair<int, int> player_pos,
                                                           std::pair<int, int> target_field,
                                                           std::vector<std::vector<int>> affected_fields) const
{
  if (abs ((int)(target_field.first - player_pos.first)) > 1 ||
      abs((int)(target_field.second - player_pos.second)) > 1 ||
      (target_field.first == player_pos.first && target_field.second == player_pos.second))
  {
    throw std::invalid_argument("[DAMAGEPATTERN] Thrust - Invalid target field");
  }
  int row_diff = target_field.first - player_pos.first;
  int col_diff = target_field.second - player_pos.second;
  std::pair<int, int> additional_field = {target_field.first + row_diff, target_field.second + col_diff};
  if (additional_field.first >= 0 && additional_field.first < static_cast<int>(affected_fields.size()) &&
      additional_field.second >= 0 && additional_field.second < static_cast<int>(affected_fields[0].size()))
  {
    affected_fields[additional_field.first][additional_field.second] = 1;
  }

  affected_fields[target_field.first][target_field.second] = 1;
  return affected_fields;
}

std::vector<std::vector<int>> DamagePattern::slashPattern(std::pair<int, int> player_pos,
                                                          std::pair<int, int> target_field,
                                                          std::vector<std::vector<int>> affected_fields) const
{
  if (abs ((int)(target_field.first - player_pos.first)) > 1 ||
      abs((int)(target_field.second - player_pos.second)) > 1 ||
      (target_field.first == player_pos.first && target_field.second == player_pos.second))
  {
    throw std::invalid_argument("[DAMAGEPATTERN] Slash - Invalid target field");
  }

  // Lets check if the target field is NOT diagonal to the player
  if((abs(target_field.first - player_pos.first) == 0 || abs(target_field.second - player_pos.second) == 0))
  {
    int row_diff = target_field.first - player_pos.first;
    int col_diff = target_field.second - player_pos.second;
    std::pair<int, int> additional_field1 = {target_field.first + col_diff, target_field.second - row_diff};
    std::pair<int, int> additional_field2 = {target_field.first - col_diff, target_field.second + row_diff};

    if (additional_field1.first >= 0 && additional_field1.first < static_cast<int>(affected_fields.size()) &&
        additional_field1.second >= 0 && additional_field1.second < static_cast<int>(affected_fields[0].size()))
    {
      affected_fields[additional_field1.first][additional_field1.second] = 1;
    }
    if (additional_field2.first >= 0 && additional_field2.first < static_cast<int>(affected_fields.size()) &&
        additional_field2.second >= 0 && additional_field2.second < static_cast<int>(affected_fields[0].size()))
    {
      affected_fields[additional_field2.first][additional_field2.second] = 1;
    }
  }
  else
  {
    // The Target field is diagonal to the player
    std::pair<int, int> additional_field1;
    std::pair<int, int> additional_field2;

    if (target_field.first > player_pos.first && target_field.second > player_pos.second)
    {
      additional_field1 = {player_pos.first, player_pos.second + 1};
      additional_field2 = {player_pos.first + 1, player_pos.second};
    }
    else if (target_field.first > player_pos.first && target_field.second < player_pos.second)
    {
      additional_field1 = {player_pos.first, player_pos.second - 1};
      additional_field2 = {player_pos.first + 1, player_pos.second};
    }
    else if (target_field.first < player_pos.first && target_field.second > player_pos.second)
    {
      additional_field1 = {player_pos.first, player_pos.second + 1};
      additional_field2 = {player_pos.first - 1, player_pos.second};
    }
    else if (target_field.first < player_pos.first && target_field.second < player_pos.second)
    {
      additional_field1 = {player_pos.first, player_pos.second - 1};
      additional_field2 = {player_pos.first - 1, player_pos.second};
    }
    else
    {
      throw std::invalid_argument("[DAMAGEPATTERN] Slash - Invalid target field (diagonal)");
    }

    if (additional_field1.first >= 0 && additional_field1.first < static_cast<int>(affected_fields.size()) &&
        additional_field1.second >= 0 && additional_field1.second < static_cast<int>(affected_fields[0].size()))
    {
      affected_fields[additional_field1.first][additional_field1.second] = 1;
    }
    if (additional_field2.first >= 0 && additional_field2.first < static_cast<int>(affected_fields.size()) &&
        additional_field2.second >= 0 && additional_field2.second < static_cast<int>(affected_fields[0].size()))
    {
      affected_fields[additional_field2.first][additional_field2.second] = 1;
    }
  }

  affected_fields[target_field.first][target_field.second] = 1;
  return affected_fields;
}

std::vector<std::vector<int>> DamagePattern::linePattern(std::pair<int, int> player_pos,
                                                         std::pair<int, int> target_field,
                                                         std::vector<std::vector<int>> affected_fields) const
{
  int row_diff = target_field.first - player_pos.first;
  int col_diff = target_field.second - player_pos.second;

  if (abs ((int)(target_field.first - player_pos.first)) > 1 ||
      abs((int)(target_field.second - player_pos.second)) > 1 ||
      (target_field.first == player_pos.first && target_field.second == player_pos.second))
  {
    throw std::invalid_argument("[DAMAGEPATTERN] Line - Invalid target field");
  }

  // Mark the target field as affected
  affected_fields[target_field.first][target_field.second] = 1;

  // Calculate the fields in the line of the target field
  std::pair<int, int> next_field = {target_field.first + row_diff, target_field.second + col_diff};

  // Check if the next field is within the bounds of the grid
  while (next_field.first >= 0 && next_field.first < static_cast<int>(affected_fields.size()) &&
         next_field.second >= 0 && next_field.second < static_cast<int>(affected_fields[0].size()))
  {
    // Mark the next field as affected
    affected_fields[next_field.first][next_field.second] = 1;
    next_field.first += row_diff;
    next_field.second += col_diff;
  }

  return affected_fields;
}

std::vector<std::vector<int>> DamagePattern::shotPattern(std::pair<int, int> target_field,
                                                         std::vector<std::vector<int>> affected_fields) const
{
  // Mark the target field as affected
  affected_fields[target_field.first][target_field.second] = 1;

  return affected_fields;
}

std::vector<std::vector<int>> DamagePattern::burstPattern(std::pair<int, int> target_field,
                                                          std::vector<std::vector<int>> affected_fields) const
{
  // Mark the target field as affected
  affected_fields[target_field.first][target_field.second] = 1;

  // Calculate the fields adjacent to the target field
  std::vector<std::pair<int, int>> adjacent_fields = {
          {target_field.first - 1, target_field.second},
          {target_field.first + 1, target_field.second},
          {target_field.first, target_field.second - 1},
          {target_field.first, target_field.second + 1}
  };

  // Check if the adjacent fields are within the bounds of the grid
  for (const auto& field : adjacent_fields)
  {
    if (field.first >= 0 && field.first < static_cast<int>(affected_fields.size()) && field.second >= 0 &&
        field.second < static_cast<int>(affected_fields[0].size()))
    {
      // Mark the adjacent field as affected
      affected_fields[field.first][field.second] = 1;
    }
  }

  return affected_fields;
}

// TODO: Only used for debugging purposes
void DamagePattern::printDamagePattern(const std::vector<std::vector<int>> &damagePattern) const
  {
  // Print the column numbers
  std::cout << "   ";
  for (int i = 0; i < static_cast<int>(damagePattern[0].size()); ++i) {
    std::cout << " " << i + 1 << "  ";
  }
  std::cout << "\n";

  // Print the top border
  std::cout << "  ";
  for (int i = 0; i < static_cast<int>(damagePattern[0].size()); ++i) {
    std::cout << "+---";
  }
  std::cout << "+\n";

  // Print the rows with cell values
  for (int i = 0; i < static_cast<int>(damagePattern.size()); ++i) {
    std::cout << i + 1 << " ";  // Print the row number
    for (int j : damagePattern[i]) {
      std::cout << "|";
      switch (j) {
        case 0:
          std::cout << "   ";
          break;
        case 1:
          std::cout << "<!>";
          break;
        case 2:
          std::cout << "<C>";
          break;
        case 3:
          std::cout << "<X>";
          break;
        default:
          std::cout << "   ";
          break;
      }
    }
    std::cout << "|\n";

    // Print the border between rows
    std::cout << "  ";
    for (int j = 0; j < static_cast<int>(damagePattern[i].size()); ++j) {
      std::cout << "+---";
    }
    std::cout << "+\n";
  }
}