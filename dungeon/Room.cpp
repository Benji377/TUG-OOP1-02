#include "Room.hpp"
#include "../entity/DeathLocation.hpp"
#include "../entity/Door.hpp"
#include "../entity/TreasureChest.hpp"
#include "../entity/character/Enemy.hpp"
#include "../entity/character/Player.hpp"
#include "../utility/Exceptions.hpp"
#include "../utility/Utils.hpp"
#include <algorithm>
#include <climits>

using std::cout;
using std::dynamic_pointer_cast;
using std::endl;
using std::make_pair;

Room::Room(int id, int width, int height)
{
  id_ = id;
  is_complete_ = false;
  for (int row_index = 0; row_index < height; row_index++)
  {
    vector<shared_ptr<Field>> row;
    for (int column_index = 0; column_index < width; column_index++) { row.push_back(std::make_shared<Field>()); }
    fields_.push_back(row);
  }
}

vector<shared_ptr<Character>> Room::getEnemies() const
{
  vector<shared_ptr<Character>> enemies;
  for (auto &row : fields_)
  {
    for (auto &fieldPtr : row)
    {
      if (fieldPtr->getEntity() && fieldPtr->getEntity()->isCharacter() && fieldPtr->getEntity()->isEnemy())
      {
        enemies.push_back(dynamic_pointer_cast<Character>(fieldPtr->getEntity()));
      }
    }
  }
  std::sort(enemies.begin(), enemies.end(), [](const shared_ptr<Character> &a, const shared_ptr<Character> &b) -> bool {
    if (a->getAbbreviation() == b->getAbbreviation()) { return a->getId() < b->getId(); }
    return a->getAbbreviation() < b->getAbbreviation();
  });
  return enemies;
}

vector<char> Room::getEnemiesAbbreviations() const
{
  vector<char> enemies_abbreviations;
  vector<shared_ptr<Character>> enemies = getEnemies();
  for (auto &enemy : enemies) { enemies_abbreviations.push_back(enemy->getAbbreviation()); }
  return enemies_abbreviations;
}

shared_ptr<Field> Room::getField(pair<int, int> position) const
{
  return fields_[(position.first - 1)][(position.second - 1)];
}

pair<int, int> Room::getFieldOfEntity(shared_ptr<Entity> entity) const
{
  int row_idx = 1;
  int column_idx = 1;
  for (auto row : fields_)
  {
    for (auto column : row)
    {
      if (column->getEntity() == entity) { return make_pair(row_idx, column_idx); }
      ++column_idx;
    }
    ++row_idx;
    column_idx = 1;
  }
  throw UnavailableItemOrEntityCommand();
  return make_pair(-1, -1);
}

vector<pair<int, int>> Room::getSurroundingFieldPositions(pair<int, int> position) const
{
  vector<pair<int, int>> surroundingFieldPositions;
  vector<pair<int, int>> directions = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
  for (const auto &dir : directions)
  {
    pair<int, int> new_pos = make_pair(position.first + dir.first, position.second + dir.second);
    if (isValidField(new_pos)) { surroundingFieldPositions.push_back(new_pos); }
  }
  return surroundingFieldPositions;
}

vector<pair<int, int>> Room::getEmptySurroundingFieldPositions(pair<int, int> position, int character_count) const
{
  vector<pair<int, int>> fields;
  int index = 0;
  int vertical = 2;
  int horizontal = 1;
  int found_empty_fields = 0;
  pair<int, int> start_positon = make_pair(position.first - 1, position.second);
  pair<int, int> cursor = start_positon;
  while (found_empty_fields < character_count)
  {
    if (index % 5 == 0)
    {
      for (int counter = 0; counter < horizontal; counter++)
      {
        if (isValidField(cursor) && getField(cursor)->getEntity() == nullptr)
        {
          fields.push_back(cursor);
          found_empty_fields++;
        }
        cursor.second++;
      }
    }
    else if (index % 5 == 1)
    {
      for (int counter = 0; counter < vertical; counter++)
      {
        if (isValidField(cursor) && getField(cursor)->getEntity() == nullptr)
        {
          fields.push_back(cursor);
          found_empty_fields++;
        }
        cursor.first++;
      }
    }
    else if (index % 5 == 2)
    {
      for (int counter = 0; counter < horizontal * 2; counter++)
      {
        if (isValidField(cursor) && getField(cursor)->getEntity() == nullptr)
        {
          fields.push_back(cursor);
          found_empty_fields++;
        }
        cursor.second--;
      }
      horizontal++;
    }
    else if (index % 5 == 3)
    {
      for (int counter = 0; counter < vertical; counter++)
      {
        if (isValidField(cursor) && getField(cursor)->getEntity() == nullptr)
        {
          fields.push_back(cursor);
          found_empty_fields++;
        }
        cursor.first--;
      }
      vertical *= 2;
    }
    else if (index % 5 == 4)
    {
      for (int counter = 0; counter < horizontal - 1; counter++)
      {
        if (isValidField(cursor) && getField(cursor)->getEntity() == nullptr)
        {
          fields.push_back(cursor);
          found_empty_fields++;
        }
        cursor.second++;
      }
      start_positon.first--;
      cursor = start_positon;
    }
    index++;
  }
  return fields;
}

bool Room::isAdjacentField(pair<int, int> field_1, pair<int, int> field_2) const
{
  if (!isValidField(field_2) || !isValidField(field_1)) { return false; }
  int distance_rows = std::abs(field_1.first - field_2.first);
  int distance_columns = std::abs(field_1.second - field_2.second);
  if (distance_rows > 1 || distance_columns > 1) { return false; }
  return true;
}

bool Room::isValidField(pair<int, int> field) const
{
  if (field.first < 1 || field.second < 1 || field.first > getHeight() || field.second > getWidth()) { return false; }
  return true;
}

void Room::printEntityInMap(shared_ptr<Entity> entity) const
{
  if (entity == nullptr)
  {
    cout << "   |";
  }
  else
  {
    shared_ptr<Door> door = dynamic_pointer_cast<Door>(entity);
    shared_ptr<TreasureChest> chest = dynamic_pointer_cast<TreasureChest>(entity);
    shared_ptr<DeathLocation> death_location = dynamic_pointer_cast<DeathLocation>(entity);
    if (door != nullptr)
    {
      cout << (door->isLocked() ? "#D" : " D");
      cout << door->getLeadsTo() << "|";
    }
    else if (chest != nullptr)
    {
      cout << (chest->isLocked() ? "#T |" : " T |");
    }
    else if (death_location != nullptr)
    {
      cout << " X |";
    }
    else
    {
      cout << " ";
      shared_ptr<Player> player = dynamic_pointer_cast<Player>(entity);
      shared_ptr<Enemy> enemy = dynamic_pointer_cast<Enemy>(entity);
      if (player != nullptr)
      {
        cout << player->getAbbreviation() << " |";
      }
      else if (enemy != nullptr)
      {
        cout << enemy->getAbbreviation() << enemy->getId() << "|";
      }
    }
  }
}

void Room::printSeparationLine() const
{
  cout << "   +";
  for (size_t index = 0; index < fields_[0].size(); index++) { cout << "---+"; }
  cout << endl;
}

void Room::setFieldEntity(shared_ptr<Entity> entity, int row, int col)
{
  fields_[row - 1][col - 1]->setEntity(entity);
}

void Room::printRoom()
{
  cout << "    ";
  for (size_t index = 0; index < fields_[0].size(); index++) { cout << " " << index + 1 << "  "; }
  cout << endl;
  printSeparationLine();
  for (size_t row_index = 0; row_index < fields_.size(); row_index++)
  {
    cout << " " << row_index + 1 << " |";
    for (size_t column_index = 0; column_index < fields_[row_index].size(); column_index++)
    {
      shared_ptr<Entity> entity = fields_[row_index][column_index]->getEntity();
      printEntityInMap(entity);
    }
    cout << endl;
    printSeparationLine();
  }
}

void Room::checkCompletion()
{
  for (const auto &row : fields_)
  {
    for (const auto &field : row)
    {
      if (field->getEntity() != nullptr)
      {
        shared_ptr<Enemy> enemy = dynamic_pointer_cast<Enemy>(field->getEntity());
        if (enemy != nullptr)
        {
          is_complete_ = false;
          return;
        }
      }
    }
  }
  is_complete_ = true;
}

void Room::openDoors()
{
  if (is_complete_)
  {
    for (auto &row : fields_)
    {
      for (auto &field : row)
      {
        shared_ptr<Door> door = dynamic_pointer_cast<Door>(field->getEntity());
        if (door != nullptr) { door->unlock(); }
      }
    }
  }
}

vector<vector<int>> Room::getLootableAsInt() const
{
  vector<vector<int>> lootable_map;
  for (const auto &row : fields_)
  {
    vector<int> lootable_row;
    for (const auto &field : row)
    {
      if (field->getEntity() != nullptr)
      {
        shared_ptr<TreasureChest> chest = dynamic_pointer_cast<TreasureChest>(field->getEntity());
        shared_ptr<DeathLocation> death_location = dynamic_pointer_cast<DeathLocation>(field->getEntity());
        if (chest != nullptr || death_location != nullptr)
        {
          lootable_row.push_back(1);
        }
        else
        {
          lootable_row.push_back(0);
        }
      }
      else
      {
        lootable_row.push_back(0);
      }
    }
    lootable_map.push_back(lootable_row);
  }
  return lootable_map;
}

int Room::getDistance(pair<int, int> start, pair<int, int> end) const
{
  return std::abs(start.first - end.first) + std::abs(start.second - end.second);
}

bool Room::hasLoot() const
{
  for (const auto &row : fields_)
  {
    for (const auto &field : row)
    {
      shared_ptr<TreasureChest> chest = dynamic_pointer_cast<TreasureChest>(field->getEntity());
      shared_ptr<DeathLocation> death_location = dynamic_pointer_cast<DeathLocation>(field->getEntity());
      if (chest != nullptr || death_location != nullptr) { return true; }
    }
  }
  return false;
}

// TODO: Combine following three functions into one
bool Room::isLootNearby(shared_ptr<Player> player) const
{
  pair<int, int> player_position = getFieldOfEntity(player);
  vector<pair<int, int>> surrounding_fields = getSurroundingFieldPositions(player_position);
  for (const auto &field : surrounding_fields)
  {
    shared_ptr<TreasureChest> chest = dynamic_pointer_cast<TreasureChest>(getField(field)->getEntity());
     shared_ptr<DeathLocation> death_location = dynamic_pointer_cast<DeathLocation>(getField(field)->getEntity());
     if (chest != nullptr || death_location != nullptr) { return true; }
  }
  return false;
}

bool Room::isEnemyNearby(shared_ptr<Player> player) const
{
  pair<int, int> player_position = getFieldOfEntity(player);
  vector<pair<int, int>> surrounding_fields = getSurroundingFieldPositions(player_position);
  for (const auto &field : surrounding_fields)
  {
    shared_ptr<Enemy> enemy = dynamic_pointer_cast<Enemy>(getField(field)->getEntity());
    if (enemy != nullptr) { return true; }
  }
  return false;
}

std::pair<int, int> Room::getClosestEnemyPosition(shared_ptr<Player> player) const
{
  pair<int, int> player_position = getFieldOfEntity(player);
  vector<std::shared_ptr<Enemy>> enemies = getAllEntitiesOfType<Enemy>();
  int shortest_distance = INT_MAX;
  pair<int, int> closest_enemy_position = make_pair(-1, -1);
  for (const auto &enemy : enemies)
  {
    pair<int, int> enemy_position = getFieldOfEntity(enemy);
    int distance = getDistance(player_position, enemy_position);
    if (distance < shortest_distance)
    {
      shortest_distance = distance;
      closest_enemy_position = enemy_position;
    }
    else if (distance == shortest_distance)
    {
      if (enemy->getHealth() < dynamic_pointer_cast<Enemy>(getField(closest_enemy_position)->getEntity())->getHealth())
      {
        closest_enemy_position = enemy_position;
      }
    }
  }
  return closest_enemy_position;
}

std::pair<int, int> Room::getLowestHealthEnemyPosition() const
{
  vector<std::shared_ptr<Enemy>> enemies = getAllEntitiesOfType<Enemy>();
  int lowest_health = INT_MAX;
  pair<int, int> lowest_health_enemy_position = make_pair(-1, -1);
  for (const auto &enemy : enemies)
  {
    pair<int, int> enemy_position = getFieldOfEntity(enemy);
    if (enemy->getHealth() < lowest_health)
    {
      lowest_health = enemy->getHealth();
      lowest_health_enemy_position = enemy_position;
    }
  }
  return lowest_health_enemy_position;
}

// TODO: Try with to insert both vectors in one
std::pair<int, int> Room::getClosestLootposition(shared_ptr<Player> player) const
{
  pair<int, int> player_position = getFieldOfEntity(player);
  vector<std::shared_ptr<TreasureChest>> chests = getAllEntitiesOfType<TreasureChest>();
  vector<std::shared_ptr<DeathLocation>> death_locations = getAllEntitiesOfType<DeathLocation>();
  int shortest_distance = INT_MAX;
  pair<int, int> closest_loot_position = make_pair(-1, -1);
  for (const auto &chest : chests)
  {
    pair<int, int> chest_position = getFieldOfEntity(chest);
    int distance = getDistance(player_position, chest_position);
    if (distance < shortest_distance)
    {
      shortest_distance = distance;
      closest_loot_position = chest_position;
    }
  }
  for (const auto &death_location : death_locations)
  {
    pair<int, int> death_location_position = getFieldOfEntity(death_location);
    int distance = getDistance(player_position, death_location_position);
    if (distance < shortest_distance)
    {
      shortest_distance = distance;
      closest_loot_position = death_location_position;
    }
  }
  return closest_loot_position;
}

vector<vector<int>> Room::getMapForPathfinding(pair<int, int> start, pair<int, int> end) const
{
  vector<vector<int>> map;
  for (const auto &row : fields_)
  {
    vector<int> row_map;
    for (const auto &field : row)
    {
      if (field->getEntity() != nullptr)
      {
        row_map.push_back(1);
      }
      else
      {
        row_map.push_back(0);
      }
    }
    map.push_back(row_map);
  }
  map[start.first - 1][start.second - 1] = 0;
  map[end.first - 1][end.second - 1] = 0;
  return map;
}

bool Room::getBestMoveToEnemy(shared_ptr<Player> player, pair<int, int>& next_position, int& distance) const
{
  pair<int, int> player_position = getFieldOfEntity(player);
  pair<int, int> closest_enemy_position = getClosestEnemyPosition(player);
  vector<vector<int>> map = getMapForPathfinding(player_position, closest_enemy_position);
  return Pathfinder::find_path(map, player_position, closest_enemy_position, next_position, distance);
}

bool Room::getBestMoveToDoor(shared_ptr<Player> player, pair<int, int>& next_position, int& distance) const
{
  pair<int, int> player_position = getFieldOfEntity(player);
  vector<shared_ptr<Door>> surrounding_fields = getAllEntitiesOfType<Door>();
  shared_ptr<Door> door = nullptr;
  for (const auto &field : surrounding_fields)
  {
    if (door == nullptr || field->getLeadsTo() > door->getLeadsTo()) { door = field; }
  }
  if (door == nullptr) { return false; }
  pair<int, int> door_position = getFieldOfEntity(door);
  vector<vector<int>> map = getMapForPathfinding(player_position, door_position);
  return Pathfinder::find_path(map, player_position, door_position, next_position, distance);
}

bool Room::getBestMoveToLoot(shared_ptr<Player> player, pair<int, int>& next_position, int& distance) const
{
  pair<int, int> player_position = getFieldOfEntity(player);
  pair<int, int> closest_loot_position = getClosestLootposition(player);
  vector<vector<int>> map = getMapForPathfinding(player_position, closest_loot_position);
  return Pathfinder::find_path(map, player_position, closest_loot_position, next_position, distance);
}