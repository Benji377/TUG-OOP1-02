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

std::vector<std::vector<int>> DamagePattern::getAffectedFields(std::pair<int, int> player_position,
    std::pair<int, int> target_field, int width, int height) const
{
  // Each Damage pattern has a different way of calculating the affected fields
  // We create a field with 0 = not affected, 1 = affected, 2 = player
  // We then return the field

  switch (pattern_)
  {
    case Pattern::HIT:
      return hitPattern(target_field, width, height);
    case Pattern::SLASH:
      return slashPattern(player_position, target_field, width, height);
    case Pattern::LINE:
      return linePattern(player_position, target_field, width, height);
    case Pattern::BURST:
      return burstPattern(target_field, width, height);
    case Pattern::SHOT:
      return shotPattern(target_field, width, height);
    case Pattern::THRUST:
      return thrustPattern(player_position, target_field, width, height);
    default:
      throw std::invalid_argument("Invalid pattern");
  }
}

std::vector<std::vector<int>> DamagePattern::hitPattern(std::pair<int, int> target_field, int width, int height) const
{
  // Only hits a single field. The target field
  std::vector<std::vector<int>> affected_fields(height, std::vector<int>(width, 0));
  affected_fields[target_field.first][target_field.second] = 1;
  return affected_fields;
}

std::vector<std::vector<int>> DamagePattern::thrustPattern(std::pair<int, int> player_position,
            std::pair<int, int> target_field, int width, int height) const
{
 // Affects two fields in a straight line, the direction is given by the target field
  std::vector<std::vector<int>> affected_fields(height, std::vector<int>(width, 0));
  int row_diff = target_field.first - player_position.first;
  int col_diff = target_field.second - player_position.second;
  if (row_diff == 0 && col_diff == 0)
  {
    throw std::invalid_argument("Invalid target field");
  }
  // Mark the target field as affected
  affected_fields[target_field.first][target_field.second] = 1;

  // Calculate the field beyond the target field in the direction of the thrust
  std::pair<int, int> beyond_target_field = {target_field.first + row_diff, target_field.second + col_diff};

  // Check if the field beyond the target field is within the bounds of the grid
  if (beyond_target_field.first >= 0 && beyond_target_field.first < height &&
      beyond_target_field.second >= 0 && beyond_target_field.second < width)
  {
    // Mark the field beyond the target field as affected
    affected_fields[beyond_target_field.first][beyond_target_field.second] = 1;
  }

  return affected_fields;
}

std::vector<std::vector<int>> DamagePattern::slashPattern(std::pair<int, int> player_position,
            std::pair<int, int> target_field, int width, int height) const
{
  std::vector<std::vector<int>> affected_fields(height, std::vector<int>(width, 0));
  int row_diff = target_field.first - player_position.first;
  int col_diff = target_field.second - player_position.second;

  if (row_diff == 0 && col_diff == 0)
  {
    throw std::invalid_argument("Invalid target field");
  }

  // Mark the target field as affected
  affected_fields[target_field.first][target_field.second] = 1;

  // Calculate the fields diagonally adjacent to the target field
  std::pair<int, int> left_diagonal_field = {target_field.first - row_diff, target_field.second - col_diff};
  std::pair<int, int> right_diagonal_field = {target_field.first + row_diff, target_field.second + col_diff};

  // Check if the diagonally adjacent fields are within the bounds of the grid
  if (left_diagonal_field.first >= 0 && left_diagonal_field.first < height &&
      left_diagonal_field.second >= 0 && left_diagonal_field.second < width)
  {
    // Mark the left diagonally adjacent field as affected
    affected_fields[left_diagonal_field.first][left_diagonal_field.second] = 1;
  }

  if (right_diagonal_field.first >= 0 && right_diagonal_field.first < height &&
      right_diagonal_field.second >= 0 && right_diagonal_field.second < width)
  {
    // Mark the right diagonally adjacent field as affected
    affected_fields[right_diagonal_field.first][right_diagonal_field.second] = 1;
  }

  // Calculate the fields diagonally adjacent to the target field in the perpendicular direction
  std::pair<int, int> perp_left_diagonal_field = {target_field.first - col_diff, target_field.second + row_diff};
  std::pair<int, int> perp_right_diagonal_field = {target_field.first + col_diff, target_field.second - row_diff};

  // Check if the diagonally adjacent fields in the perpendicular direction are within the bounds of the grid
  if (perp_left_diagonal_field.first >= 0 && perp_left_diagonal_field.first < height &&
      perp_left_diagonal_field.second >= 0 && perp_left_diagonal_field.second < width)
  {
    // Mark the left diagonally adjacent field in the perpendicular direction as affected
    affected_fields[perp_left_diagonal_field.first][perp_left_diagonal_field.second] = 1;
  }

  if (perp_right_diagonal_field.first >= 0 && perp_right_diagonal_field.first < height &&
      perp_right_diagonal_field.second >= 0 && perp_right_diagonal_field.second < width)
  {
    // Mark the right diagonally adjacent field in the perpendicular direction as affected
    affected_fields[perp_right_diagonal_field.first][perp_right_diagonal_field.second] = 1;
  }

  return affected_fields;
}

std::vector<std::vector<int>> DamagePattern::linePattern(std::pair<int, int> player_position,
            std::pair<int, int> target_field, int width, int height) const
{
  std::vector<std::vector<int>> affected_fields(height, std::vector<int>(width, 0));
  int row_diff = target_field.first - player_position.first;
  int col_diff = target_field.second - player_position.second;

  if (row_diff == 0 && col_diff == 0)
  {
    throw std::invalid_argument("Invalid target field");
  }

  // Mark the target field as affected
  affected_fields[target_field.first][target_field.second] = 1;

  // Calculate the fields in the line of the target field
  std::pair<int, int> next_field = {target_field.first + row_diff, target_field.second + col_diff};

  // Check if the next field is within the bounds of the grid
  while (next_field.first >= 0 && next_field.first < height &&
         next_field.second >= 0 && next_field.second < width)
  {
    // Mark the next field as affected
    affected_fields[next_field.first][next_field.second] = 1;
    next_field.first += row_diff;
    next_field.second += col_diff;
  }

  return affected_fields;
}

std::vector<std::vector<int>> DamagePattern::shotPattern(std::pair<int, int> target_field, int width, int height) const
{
  // Initialize all fields as not affected
  std::vector<std::vector<int>> affected_fields(height, std::vector<int>(width, 0));

  // Mark the target field as affected
  affected_fields[target_field.first][target_field.second] = 1;

  return affected_fields;
}

std::vector<std::vector<int>> DamagePattern::burstPattern(std::pair<int, int> target_field, int width, int height) const
{
  // Initialize all fields as not affected
  std::vector<std::vector<int>> affected_fields(height, std::vector<int>(width, 0));

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
    if (field.first >= 0 && field.first < height && field.second >= 0 && field.second < width)
    {
      // Mark the adjacent field as affected
      affected_fields[field.first][field.second] = 1;
    }
  }

  return affected_fields;
}

void DamagePattern::printDamagePattern(const std::vector<std::vector<int>> &damagePattern) const
  {
  // Print the column numbers
  std::cout << "   ";
  for (int i = 0; i < damagePattern[0].size(); ++i) {
    std::cout << " " << i + 1 << "  ";
  }
  std::cout << "\n";

  // Print the top border
  std::cout << "  ";
  for (int i = 0; i < damagePattern[0].size(); ++i) {
    std::cout << "+---";
  }
  std::cout << "+\n";

  // Print the rows with cell values
  for (int i = 0; i < damagePattern.size(); ++i) {
    std::cout << i + 1 << " ";  // Print the row number
    for (int j = 0; j < damagePattern[i].size(); ++j) {
      std::cout << "|";
      switch (damagePattern[i][j]) {
        case 0:
          std::cout << "   ";
          break;
        case 1:
          std::cout << "<!>";
          break;
        default:
          std::cout << "   ";
          break;
      }
    }
    std::cout << "|\n";

    // Print the border between rows
    std::cout << "  ";
    for (int j = 0; j < damagePattern[i].size(); ++j) {
      std::cout << "+---";
    }
    std::cout << "+\n";
  }
}