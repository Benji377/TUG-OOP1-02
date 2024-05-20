#include "Room.hpp"
#include "../utility/Utils.hpp"
#include "../entity/Door.hpp"
#include "../entity/TreasureChest.hpp"
#include "../entity/DeathLocation.hpp"
#include "../entity/character/Enemy.hpp"
#include "../entity/character/Player.hpp"
#include "../utility/Exceptions.hpp"
#include <algorithm>


Room::Room(int id, int width, int height)
{
  id_ = id;
  is_complete_ = false;
  for (int i = 0; i < height; i++)
  {
    vector<shared_ptr<Field>> row;
    for (int j = 0; j < width; j++)
    {
      row.push_back(std::make_shared<Field>());
    }
    fields_.push_back(row);
  }
}

void Room::setFieldEntity(shared_ptr<Entity> entity, int row, int col)
{
  fields_[row - 1][col - 1]->setEntity(entity);
}

std::pair<int, int> Room::getFieldOfEntity(shared_ptr<Entity> entity)
{
  int row_idx = 1;
  int column_idx = 1;

  for(auto row : fields_)
  {
    for(auto column : row)
    {
      if(column->getEntity() == entity)
      {
        return std::make_pair(row_idx, column_idx);
      }
      ++column_idx;
    }
    ++row_idx;
    column_idx = 1;
  }

  throw UnavailableItemOrEntityCommand();
  return std::make_pair(-1, -1);
}


void Room::printRoom()
{
  std::cout << "    ";
  for (size_t i = 0; i < fields_[0].size(); i++)
  {
    std::cout << " " << i + 1 << "  ";
  }
  std::cout << std::endl;
  printSeparationLine();
  for (size_t i = 0; i < fields_.size(); i++)
  {
    std::cout << " " << i + 1 << " |";
    for (size_t j = 0; j < fields_[i].size(); j++)
    {
      shared_ptr<Entity> entity = fields_[i][j]->getEntity();
      printEntityInMap(entity);
    }
    std::cout << std::endl;
    printSeparationLine();
  }
}

void Room::printEntityInMap(shared_ptr<Entity> entity)
{
  if (entity == nullptr)
  {
    std::cout << "   |";
  }
  else
  {
    std::shared_ptr<Door> door = std::dynamic_pointer_cast<Door>(entity);
    std::shared_ptr<TreasureChest> chest = std::dynamic_pointer_cast<TreasureChest>(entity);
    std::shared_ptr<DeathLocation> death_location = std::dynamic_pointer_cast<DeathLocation>(entity);
    if (door != nullptr)
    {
      std::cout << (door->isLocked() ? "#D" : " D");
      std::cout << door->getLeadsTo() << "|";
    }
    else if (chest != nullptr)
    {
      std::cout << (chest->isLocked() ? "#T |" : " T |");
    }
    else if (death_location != nullptr)
    {
      std::cout << " X |";
    }
    else
    {
      std::cout << " ";
      std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(entity);
      std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(entity);
      if (player != nullptr)
      {
        std::cout << player->getAbbreviation() << " |";
      }
      else if (enemy != nullptr)
      {
        std::cout << enemy->getAbbreviation() << enemy->getId() << "|";
      }
    }
  }
}

void Room::printSeparationLine()
{
  std::cout << "   +";
  for (size_t i = 0; i < fields_[0].size(); i++)
  {
    std::cout << "---+";
  }
  std::cout << std::endl;
}

std::vector<std::shared_ptr<Character>> Room::getEnemies()
{
  std::vector<std::shared_ptr<Character>> enemies;

  for (auto& row : fields_)
  {
    for (auto& fieldPtr : row)
    {
      //Checks first if it isn't the nullptr, then dereferences and checks if it's a character
      if(fieldPtr->getEntity() && fieldPtr->getEntity()->isCharacter() && fieldPtr->getEntity()->isEnemy())
      {
        enemies.push_back(std::dynamic_pointer_cast<Character>(fieldPtr->getEntity()));
      }
    }
  }

  // Sort the enemies vector
  std::sort(enemies.begin(), enemies.end(),
    [](const std::shared_ptr<Character>& a, const std::shared_ptr<Character>& b) -> bool
    {
      if (a->getAbbreviation() == b->getAbbreviation())
      {
        return a->getId() < b->getId();
      }
      return a->getAbbreviation() < b->getAbbreviation();
    });

  return enemies;
}

std::vector<char> Room::getEnemiesAbbreviations()
{
  std::vector<char> enemies_abbreviations;
  std::vector<std::shared_ptr<Character>> enemies = getEnemies();
  for (auto& enemy : enemies)
  {
    enemies_abbreviations.push_back(enemy->getAbbreviation());
  }
  return enemies_abbreviations;
}

std::vector<std::pair<int, int>> Room::getSurroundingFieldPositions(std::pair<int, int> position)
{
  std::vector<std::pair<int, int>> surroundingFieldPositions;
  std::vector<std::pair<int, int>> directions = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1},{-1, -1}};
  for (const auto& dir : directions)
  {
    std::pair<int, int> new_pos = std::make_pair(position.first + dir.first, position.second + dir.second);

    if (isValidField(new_pos))
    {
      surroundingFieldPositions.push_back(new_pos);
    }
  }
  return surroundingFieldPositions;
}

std::vector<std::pair<int, int>> Room::getEmptySurroundingFieldPositions(std::pair<int, int> position,
  int character_count)
{
  std::vector<std::pair<int, int>> fields;
  int i = 0;
  int vertical = 2;
  int horizontal = 1;
  int found_empty_fields = 0;
  std::pair<int, int> start_positon = std::make_pair(position.first - 1, position.second);
  std::pair<int, int> cursor = start_positon;
  while (found_empty_fields < character_count)
  {
    if (i % 5 == 0)
    {
      for (int j = 0; j < horizontal; j++)
      {
        if (isValidField(cursor) && getField(cursor)->getEntity() == nullptr)
        {
          fields.push_back(cursor);
          found_empty_fields++;
        }
        cursor.second++;
      }
    }
    else if (i % 5 == 1)
    {
      for (int j = 0; j < vertical; j++)
      {
        if (isValidField(cursor) && getField(cursor)->getEntity() == nullptr)
        {
          fields.push_back(cursor);
          found_empty_fields++;
        }
        cursor.first++;
      }
    }
    else if (i % 5 == 2)
    {
      for (int j = 0; j < horizontal * 2; j++)
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
    else if (i % 5 == 3)
    {
      for (int j = 0; j < vertical; j++)
      {
        if (isValidField(cursor) && getField(cursor)->getEntity() == nullptr)
        {
          fields.push_back(cursor);
          found_empty_fields++;
        }
        cursor.first--;
      }
      vertical*=2;
    }
    else if (i % 5 == 4)
    {
      for (int j = 0; j < horizontal - 1; j++)
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
    i++;
  }
  return fields;
}

bool Room::isAdjacentField(std::pair<int,int> field_1, std::pair<int,int> field_2)
{
  if(!isValidField(field_2) || !isValidField(field_1))
  {
    return false;
  }

  //Check their distance
  int distance_rows = std::abs(field_1.first - field_2.first);
  int distance_columns = std::abs(field_1.second - field_2.second);

  if(distance_rows > 1 || distance_columns > 1)
  {
    return false;
  }

  return true;

}

void Room::checkCompletion()
{
  for (const auto& row : fields_)
  {
    for (const auto& field : row)
    {
      if (field->getEntity() != nullptr)
      {
        std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(field->getEntity());
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
    for (auto& row : fields_)
    {
      for (auto& field : row)
      {
        std::shared_ptr<Door> door = std::dynamic_pointer_cast<Door>(field->getEntity());
        if (door != nullptr)
        {
          door->unlock();
        }
      }
    }
  }
}

bool Room::isValidField(std::pair<int,int> field)
{
  if(field.first < 1 || field.second < 1 || field.first > getHeight()
    || field.second > getWidth())
  {
    return false;
  }

  return true;
}