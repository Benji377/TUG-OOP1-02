#include "Room.hpp"
#include "../utility/Utils.hpp"
#include "../entity/Door.hpp"
#include "../entity/TreasureChest.hpp"
#include "../entity/character/Enemy.hpp"
#include "../entity/character/Player.hpp"


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

//std::map<std::shared_ptr<Entity>, std::string> Room::getCharacters()
std::vector<std::shared_ptr<Entity>> Room::getCharacters()
{



  /*
  int row_nr;
  int column_nr;

  std::map<std::shared_ptr<Entity>, std::string> ptr_and_field;

  for (auto& row : fields_)
  {
    row_nr++;

    for (auto& fieldPtr : row)
    {
      column_nr++;

        if(fieldPtr->getEntity() == std::dynamic_pointer_cast<Entity>(fieldPtr->getEntity()))
        {
           ptr_and_field[fieldPtr->getEntity()] = std::to_string(row_nr) + "," + std::to_string(column_nr);
        }
    }
  }
  return ptr_and_field; */
}