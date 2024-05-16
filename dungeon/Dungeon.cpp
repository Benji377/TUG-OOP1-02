#include "Dungeon.hpp"
#include "../utility/Utils.hpp"
#include "../entity/Door.hpp"
#include "../entity/character/Enemy.hpp"
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

void Dungeon::moveToRoom(int room_id)
{
  int current_room_id = current_room_->getId();
  vector<shared_ptr<Player>> players = exitCurrentRoom();
  current_room_ = rooms_[room_id];
  enterCurrentRoom(current_room_id, players);
  printCurrentRoom();
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
          current_room_->setFieldEntity(nullptr, i, j);
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
      int i = 1;
      int player_count = 0;
      while ((size_t)player_count < players.size())
      {
        vector<shared_ptr<Field>> surrounding_fields = current_room_->getSurroundingFields(door_position, i);
        for (size_t j = 0; j < surrounding_fields.size(); j++)
        {
          if (surrounding_fields[j]->getEntity() == nullptr)
          {
            surrounding_fields[j]->setEntity(players[player_count]);
            player_count++;
            if ((size_t)player_count == players.size())
            {
              break;
            }
          }
        }
        i++;
      }
    }
  }
}

void Dungeon::characterMove(shared_ptr<Character> character, std::pair<int, int> position)
{
  std::pair<int, int> character_position = current_room_->getFieldOfEntity(character);
  current_room_->setFieldEntity(nullptr, character_position.first, character_position.second);
  current_room_->setFieldEntity(character, position.first, position.second);
}

vector<AttackedCharacter> Dungeon::characterAttack(shared_ptr<Character> attacker, int damage, std::pair <int, int> target_field)
{
  vector<AttackedCharacter> attacked_characters;
  std::pair<int, int> attacker_position = current_room_->getFieldOfEntity(attacker);
  DamageType damage_type = attacker->getWeapon()->getDamageType();
  vector<vector<int>> affected_fields = attacker->getWeapon()->getDamagePattern()->getAffectedFields(attacker_position,
    target_field, current_room_->getWidth(), current_room_->getHeight());
  for (size_t i = 0; i < affected_fields.size(); i++)
  {
    for (size_t j = 0; j < affected_fields[i].size(); j++)
    {
      if (affected_fields[i][j] == 1)
      {
        shared_ptr<Character> target =
          std::dynamic_pointer_cast<Character>(current_room_->getField({i, j})->getEntity());
        if (target != nullptr)
        {
          int lost_health = target->takeDamage(damage, damage_type);
          if (target->isDead())
          {
            killCharacter(target);
          }

          int armor_value = 0;
          if(target->getArmor() != nullptr)
          {
            armor_value = target->getArmor()->getArmorValue(); //fixed case where target has no armor
          }

          AttackedCharacter attacked_character = { {i, j}, "", lost_health, damage,
              target->getResistantTo() == damage_type ? 50 : 100, armor_value,
              target->isDead() };
          shared_ptr<Player> attacked_player = std::dynamic_pointer_cast<Player>(target);
          shared_ptr<Enemy> attacked_enemy = std::dynamic_pointer_cast<Enemy>(target);
          if (attacked_player != nullptr)
          {
            attacked_character.character_name = attacked_player->getName();
          }
          else if (attacked_enemy != nullptr)
          {
            std::string enemy_name = attacked_enemy->getTypeName() + " " + std::to_string(attacked_enemy->getId());
            attacked_character.character_name = enemy_name;
          }
          attacked_characters.push_back(attacked_character);
        }
      }
    }
  }
  sort(attacked_characters.begin(), attacked_characters.end(),
    [](AttackedCharacter a, AttackedCharacter b) -> bool { return a.position < b.position; });
  return attacked_characters;
}

void Dungeon::lootEntity(std::shared_ptr<Player> player, std::shared_ptr<Entity> entity)
{
  std::map<string, int> loot = entity->getLoot();
  int ret = player->getInventory()->parseInventory(loot);
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