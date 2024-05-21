#include "Dungeon.hpp"
#include "../utility/Utils.hpp"
#include "../entity/Door.hpp"
#include "../entity/TreasureChest.hpp"
#include "../entity/DeathLocation.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <algorithm>

using std::map;
using std::shared_ptr;
using std::string;

Dungeon::Dungeon(const char *file_path)
{
  boss_dead_ = false;

  std::ifstream file(file_path);
  if (file.is_open())
  {
    string line;
    std::getline(file, line);
    rooms_.push_back(nullptr); // Add a dummy room at index 0
    while (std::getline(file, line))
    {
      // Split the line by room and entities
      vector<string> entities = Utils::splitString(line, ";");
      string temp_room_info = entities[0];
      entities.erase(entities.begin());
      // Create a room object
      vector<string> room_info = Utils::splitString(temp_room_info, ":");
      int room_id = std::stoi(room_info[0]);
      vector<string> room_dimensions = Utils::splitString(room_info[1], ",");
      int room_width = std::stoi(room_dimensions[0]);
      int room_height = std::stoi(room_dimensions[1]);
      rooms_.push_back(std::make_shared<Room>(room_id, room_width, room_height));
      if (room_id == 1)
      {
        current_room_ = rooms_[room_id];
      }
      // Add entities to the room
      for (string entity : entities)
      {
        if (entity.empty())
        {
          continue;
        }
        vector<string> entity_info = Utils::splitString(entity, ":");
        char entity_abbreviation = entity_info[0][0];
        vector<string> entity_position = Utils::splitString(entity_info[1], ",");
        int entity_row = std::stoi(entity_position[0]);
        int entity_col = std::stoi(entity_position[1]);
        vector<string> parameters = Utils::splitString(entity_info[2], ",");
        // TEMPORARY: Create entities based on their abbreviation (TODO: Implement factory pattern)
        shared_ptr<Door> door;
        shared_ptr<Enemy> enemy;
        shared_ptr<TreasureChest> chest;
        map<string, int> treasure_chest_loot;
        switch (entity_abbreviation)
        {
        case 'D':
          door = std::make_shared<Door>(std::stoi(parameters[0]));
          rooms_[room_id]->setFieldEntity(door, entity_row, entity_col);
          break;
        case 'Z':
        case 'G':
        case 'L':
          enemy = std::make_shared<Enemy>(std::stoi(parameters[0]), entity_abbreviation);
          rooms_[room_id]->setFieldEntity(enemy, entity_row, entity_col);
          break;
        case 'T':
          for (size_t i = 1; i < parameters.size(); i+=2)
          {
            treasure_chest_loot[parameters[i]] = std::stoi(parameters[i+1]);
          }
          chest = std::make_shared<TreasureChest>(std::stoi(parameters[0]), treasure_chest_loot);
          rooms_[room_id]->setFieldEntity(chest, entity_row, entity_col);
          break;
        default:
          break;
        }
      }
    }
    file.close();
  }
  occured_enemy_types_ = {};
}

void Dungeon::printCurrentRoom()
{
  current_room_->printRoom();
}

std::shared_ptr<Room> Dungeon::getCurrentRoom()
{
  return current_room_;
}

int Dungeon::getCompletedRoomsCount() const
{
  int count = 0;
  for (auto room : rooms_)
  {
    if (room != nullptr)
    {
      if (room->isComplete())
      {
        count++;
      }
    }
  }
  return count;
}

int Dungeon::moveToRoom(int room_id)
{
  if (room_id == 0)
  {
    return 1;
  }
  int current_room_id = current_room_->getId();
  vector<shared_ptr<Player>> players = exitCurrentRoom();
  current_room_ = rooms_[room_id];
  enterCurrentRoom(current_room_id, players);
  return 0;
}

vector<shared_ptr<Player>> Dungeon::exitCurrentRoom()
{
  vector<vector<shared_ptr<Field>>> fields = current_room_->getFields();
  vector<shared_ptr<Player>> players;
  for (size_t i = 0; i < fields.size(); i++)
  {
    for (size_t j = 0; j < fields[i].size(); j++)
    {
      if (fields[i][j]->getEntity() != nullptr)
      {
        shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(fields[i][j]->getEntity());
        if (player != nullptr)
        {
          current_room_->setFieldEntity(nullptr, i + 1, j + 1);
          player->setResistance(DamageType::NONE);
          players.push_back(player);
        }
      }
    }
  }
  sort(players.begin(), players.end(), [](shared_ptr<Player> a, shared_ptr<Player> b) ->
    bool { return a->getId() < b->getId(); });
  return players;
}

void Dungeon::enterCurrentRoom(int door_id, vector<shared_ptr<Player>> players)
{
  vector<vector<shared_ptr<Field>>> fields = current_room_->getFields();
  vector<shared_ptr<Door>> door_fields = current_room_->getAllEntitiesOfType<Door>();
  for (auto door : door_fields)
  {
    if (door->getId() == door_id)
    {
      std::pair<int, int> door_position = current_room_->getFieldOfEntity(door);
      vector<std::pair<int, int>> surrounding_fields_pos = current_room_->getEmptySurroundingFieldPositions(door_position, players.size());
      int i = 0;
      for (auto player : players)
      {
        current_room_->setFieldEntity(player, surrounding_fields_pos[i].first, surrounding_fields_pos[i].second);
        i++;
      }
      break;
    }
  }
}

void Dungeon::characterMove(shared_ptr<Character> character, std::pair<int, int> position)
{
  std::pair<int, int> character_position = current_room_->getFieldOfEntity(character);
  current_room_->setFieldEntity(nullptr, character_position.first, character_position.second);
  current_room_->setFieldEntity(character, position.first, position.second);
}

void Dungeon::moveToRandomField(std::shared_ptr<Enemy> enemy) {
  std::pair<int, int> enemy_position = current_room_->getFieldOfEntity(enemy);
  current_room_->setFieldEntity(nullptr, enemy_position.first, enemy_position.second);
  std::vector<std::pair<int, int>> surrounding_fields = current_room_->getSurroundingFieldPositions(enemy_position);
  Dice dice = Dice(surrounding_fields.size(), 1);
  int random_index = dice.roll() - 1;
  std::pair<int, int> random_position = surrounding_fields[random_index];
  if (current_room_->getField(random_position)->getEntity() == nullptr)
  {
    current_room_->setFieldEntity(enemy, random_position.first, random_position.second);
  }
  else
  {
    std::vector<std::pair<int, int>> new_surrounding_fields =
      current_room_->getEmptySurroundingFieldPositions(random_position, 1);
    current_room_->setFieldEntity(enemy, new_surrounding_fields[0].first, new_surrounding_fields[0].second);
  }

}

vector<AttackedField> Dungeon::characterAttack(shared_ptr<Character> attacker, int damage, std::pair <int, int> target_field)
{
  vector<AttackedField> attacked_fields;
  std::pair<int, int> attacker_position = current_room_->getFieldOfEntity(attacker);

  shared_ptr<Weapon> weapon;
  if(std::dynamic_pointer_cast<Player>(attacker) != nullptr) //otherwise it only takes thge getWeapon() function from Character which chage Quarterstaffs depending on the holder
  {
    shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(attacker);
    weapon = player->getWeapon();
  }
  else
  {
    weapon = attacker->getWeapon();
  }

  DamageType damage_type = weapon->getDamageType();
  vector<vector<int>> affected_fields = weapon->getDamagePattern()->getAffectedFields(attacker_position,
    target_field, current_room_->getWidth(), current_room_->getHeight());
  for (size_t i = 0; i < affected_fields.size(); i++)
  {
    for (size_t j = 0; j < affected_fields[i].size(); j++)
    {
      if (affected_fields[i][j] == 1)
      {
        shared_ptr<Character> target =
          std::dynamic_pointer_cast<Character>(current_room_->getField({i + 1, j + 1})->getEntity());
        AttackedField attacked_field = AttackedField({i + 1, j + 1});
        if (target != nullptr)
        {
          int lost_health = target->takeDamage(damage, damage_type);
          if (target->isDead())
          {
            killCharacter(target);
          }

          int armor_value = target->getBaseArmor();
          if(target->getArmor() != nullptr)
          {
            armor_value = target->getArmor()->getArmorValue();

            if(target->getArmor()->getAbbreviation() == "LARM")
            {
              armor_value = 1 + target->getVitality();
            }
            else if(target->getArmor()->getAbbreviation() == "BPLT")
            {
              armor_value = 4 + std::min(target->getVitality(), 2);
            }

          }

          attacked_field.setCharacterWithoutName(lost_health, damage,
            target->getResistantTo() == damage_type ? 50 : 100, armor_value, target->isDead());
          shared_ptr<Player> attacked_player = std::dynamic_pointer_cast<Player>(target);
          shared_ptr<Enemy> attacked_enemy = std::dynamic_pointer_cast<Enemy>(target);
          if (attacked_player != nullptr)
          {
            attacked_field.setName(attacked_player->getName());
          }
          else if (attacked_enemy != nullptr)
          {
            std::string enemy_name = attacked_enemy->getTypeName() + " " + std::to_string(attacked_enemy->getId());
            attacked_field.setName(enemy_name);

            if(attacked_enemy->is_boss() && attacked_enemy->isDead())
            {
              boss_dead_ = true;
              current_room_->setComplete(true);
            }
          }
        }
        attacked_fields.push_back(attacked_field);
      }
    }
  }
  sort(attacked_fields.begin(), attacked_fields.end(),
    [](AttackedField a, AttackedField b) -> bool { return a.getPosition() < b.getPosition(); });
  return attacked_fields;
}

void Dungeon::lootEntity(std::shared_ptr<Player> player, std::shared_ptr<Entity> entity)
{
  std::map<string, int> loot = entity->getLoot();
  int ret = player->getInventory()->parseInventory(loot, player->getStrength(), player->getVitality());
  if (ret == 1)
  {
    std::cout << "The entity contains an unknown item. The loot could not be parsed." << std::endl;
  }
  std::pair<int, int> position = getCurrentRoom()->getFieldOfEntity(entity);
  getCurrentRoom()->setFieldEntity(nullptr, position.first, position.second);
}

void Dungeon::killCharacter(std::shared_ptr<Character> character)
{
  std::pair<int, int> position = getCurrentRoom()->getFieldOfEntity(character);
  getCurrentRoom()->setFieldEntity(nullptr, position.first, position.second);
  std::map<string, int> loot = character->getInventory()->getInventoryMapped();
  shared_ptr<DeathLocation> death_location = std::make_shared<DeathLocation>(loot);
  getCurrentRoom()->setFieldEntity(death_location, position.first, position.second);
  shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(character);
  if (player != nullptr)
  {
    player->kill();
  }
}