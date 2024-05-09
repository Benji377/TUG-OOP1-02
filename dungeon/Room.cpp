#include "Room.hpp"
#include "../utility/Utils.hpp"
#include "../entity/Door.hpp"
#include "../entity/TreasureChest.hpp"
#include "../entity/character/Enemy.hpp"
#include "../entity/character/Player.hpp"
#include "../utility/Exceptions.hpp"


Room::Room(int id, int width, int height)
{
  id_ = id;
  enemy_count_ = 0;
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
  int row_idx = 0;
  int column_idx = 0;

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
    column_idx = 0;
  }


  std::cout << "err, entity in field not recognised" << std::endl;

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
      printEntitie(entity);
    }
    std::cout << std::endl;
    printSeparationLine();
  }
}

// TODO: Print out the enemy health
void Room::printEntitie(shared_ptr<Entity> entity)
{
  if (entity == nullptr)
  {
    std::cout << "   |";
  }
  else
  {
    std::shared_ptr<Door> door = std::dynamic_pointer_cast<Door>(entity);
    std::shared_ptr<TreasureChest> chest = std::dynamic_pointer_cast<TreasureChest>(entity);
    if (door != nullptr)
    {
      std::cout << (door->isLocked() ? "#D" : " D");
      std::cout << door->getLeadsTo() << "|";
    }
    else if (chest != nullptr)
    {
      std::cout << (chest->isLocked() ? "#T |" : " T |");
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
  return enemies;
}