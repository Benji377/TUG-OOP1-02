#include "Dungeon.hpp"
#include "../entity/DeathLocation.hpp"
#include "../entity/Door.hpp"
#include "../entity/TreasureChest.hpp"
#include "../utility/Utils.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

using std::dynamic_pointer_cast;
using std::make_shared;
using std::map;
using std::stoi;
using std::string;

Dungeon::Dungeon(const char *file_path)
{
  boss_dead_ = false;
  std::ifstream file(file_path);
  if (file.is_open())
  {
    string line;
    std::getline(file, line);
    rooms_.push_back(nullptr);// Add a dummy room at index 0
    while (std::getline(file, line))
    {
      // Split the line by room and entities
      vector<string> entities = Utils::splitString(line, ";");
      string temp_room_info = entities[0];
      entities.erase(entities.begin());
      // Create a room object
      vector<string> room_info = Utils::splitString(temp_room_info, ":");
      int room_id = stoi(room_info[0]);
      vector<string> room_dimensions = Utils::splitString(room_info[1], ",");
      int room_width = stoi(room_dimensions[0]);
      int room_height = stoi(room_dimensions[1]);
      rooms_.push_back(make_shared<Room>(room_id, room_width, room_height));
      if (room_id == 1) { current_room_ = rooms_[room_id]; }
      // Add entities to the room
      for (string entity : entities)
      {
        if (entity.empty()) { continue; }
        vector<string> entity_info = Utils::splitString(entity, ":");
        char entity_abbreviation = entity_info[0][0];
        vector<string> entity_position = Utils::splitString(entity_info[1], ",");
        int entity_row = stoi(entity_position[0]);
        int entity_col = stoi(entity_position[1]);
        vector<string> parameters = Utils::splitString(entity_info[2], ",");
        shared_ptr<Door> door;
        shared_ptr<Enemy> enemy;
        shared_ptr<TreasureChest> chest;
        map<string, int> treasure_chest_loot;
        switch (entity_abbreviation)
        {
          case 'D':
            door = make_shared<Door>(stoi(parameters[0]));
            rooms_[room_id]->setFieldEntity(door, entity_row, entity_col);
            break;
          case 'Z':
          case 'G':
          case 'L':
            enemy = make_shared<Enemy>(stoi(parameters[0]), entity_abbreviation);
            rooms_[room_id]->setFieldEntity(enemy, entity_row, entity_col);
            break;
          case 'T':
            for (size_t i = 1; i < parameters.size(); i += 2)
            {
              treasure_chest_loot[parameters[i]] = stoi(parameters[i + 1]);
            }
            chest = make_shared<TreasureChest>(stoi(parameters[0]), treasure_chest_loot);
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

shared_ptr<Room> Dungeon::getCurrentRoom() const
{
  return current_room_;
}

bool Dungeon::playersInRoomAreDead() const
{
  vector<shared_ptr<Player>> players = current_room_->getAllEntitiesOfType<Player>();
  for (auto player : players)
  {
    if (!player->isDead()) { return false; }
  }
  return true;
}

void Dungeon::printCurrentRoom()
{
  current_room_->printRoom();
}

int Dungeon::getCompletedRoomsCount() const
{
  int count = 0;
  for (auto room : rooms_)
  {
    if (room != nullptr)
    {
      if (room->isComplete()) { count++; }
    }
  }
  return count;
}

int Dungeon::moveToRoom(int room_id)
{
  if (room_id == 0) { return 1; }
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
  for (size_t row_index = 0; row_index < fields.size(); row_index++)
  {
    for (size_t column_index = 0; column_index < fields[row_index].size(); column_index++)
    {
      if (fields[row_index][column_index]->getEntity() != nullptr)
      {
        shared_ptr<Player> player = dynamic_pointer_cast<Player>(fields[row_index][column_index]->getEntity());
        if (player != nullptr)
        {
          current_room_->setFieldEntity(nullptr, row_index + 1, column_index + 1);
          player->setResistance(DamageType::NONE);
          players.push_back(player);
        }
      }
    }
  }
  sort(players.begin(), players.end(),
       [](shared_ptr<Player> a, shared_ptr<Player> b) -> bool { return a->getId() < b->getId(); });
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
      pair<int, int> door_position = current_room_->getFieldOfEntity(door);
      vector<pair<int, int>> surrounding_fields_pos =
              current_room_->getEmptySurroundingFieldPositions(door_position, players.size());
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

void Dungeon::characterMove(shared_ptr<Character> character, pair<int, int> position)
{
  pair<int, int> character_position = current_room_->getFieldOfEntity(character);
  current_room_->setFieldEntity(nullptr, character_position.first, character_position.second);
  current_room_->setFieldEntity(character, position.first, position.second);
}

void Dungeon::moveToRandomField(shared_ptr<Enemy> enemy)
{
  pair<int, int> enemy_position = current_room_->getFieldOfEntity(enemy);
  current_room_->setFieldEntity(nullptr, enemy_position.first, enemy_position.second);
  vector<pair<int, int>> surrounding_fields = current_room_->getSurroundingFieldPositions(enemy_position);
  Dice dice = Dice(surrounding_fields.size(), 1);
  int random_index = dice.roll() - 1;
  pair<int, int> random_position = surrounding_fields[random_index];
  if (current_room_->getField(random_position)->getEntity() == nullptr)
  {
    current_room_->setFieldEntity(enemy, random_position.first, random_position.second);
  }
  else
  {
    vector<pair<int, int>> new_surrounding_fields =
            current_room_->getEmptySurroundingFieldPositions(random_position, 1);
    current_room_->setFieldEntity(enemy, new_surrounding_fields[0].first, new_surrounding_fields[0].second);
  }
}

vector<AttackedField> Dungeon::characterAttack(shared_ptr<Character> attacker, int damage, pair<int, int> target_field)
{
  vector<AttackedField> attacked_fields;
  pair<int, int> attacker_position = current_room_->getFieldOfEntity(attacker);
  shared_ptr<Weapon> weapon;
  if (dynamic_pointer_cast<Player>(attacker) != nullptr)
  {
    shared_ptr<Player> player = dynamic_pointer_cast<Player>(attacker);
    weapon = player->getWeapon();
  }
  else
  {
    weapon = attacker->getWeapon();
  }
  DamageType damage_type = weapon->getDamageType();
  vector<vector<int>> affected_fields = weapon->getDamagePattern()->getAffectedFields(
          attacker_position, target_field, current_room_->getWidth(), current_room_->getHeight());
  for (size_t row_index = 0; row_index < affected_fields.size(); row_index++)
  {
    for (size_t column_index = 0; column_index < affected_fields[row_index].size(); column_index++)
    {
      if (affected_fields[row_index][column_index] == 1)
      {
        shared_ptr<Character> target =
                dynamic_pointer_cast<Character>(current_room_->getField({row_index + 1, column_index + 1})->getEntity());
        AttackedField attacked_field = AttackedField({row_index + 1, column_index + 1});
        if (target != nullptr && !boss_dead_ && !playersInRoomAreDead())
        {
          int lost_health = target->takeDamage(damage, damage_type);
          if (target->isDead()) { killCharacter(target); }
          int armor_value = target->getBaseArmor();
          if (target->getArmor() != nullptr)
          {
            armor_value = target->getArmor()->getArmorValue();
            if (target->getArmor()->getAbbreviation() == "LARM")
            {
              armor_value = 1 + target->getVitality();
            }
            else if (target->getArmor()->getAbbreviation() == "BPLT")
            {
              armor_value = 4 + std::min(target->getVitality(), 2);
            }
          }
          shared_ptr<Player> target_player = dynamic_pointer_cast<Player>(target);
          shared_ptr<Enemy> target_enemy = dynamic_pointer_cast<Enemy>(target);
          CharacterType character_type;
          if (target_player != nullptr)
          {
            character_type = CharacterType::PLAYER;
          }
          else if (target_enemy != nullptr)
          {
            character_type = CharacterType::ENEMY;
          }
          else
          {
            character_type = CharacterType::NONE;
          }
          attacked_field.setCharacterWithoutName(character_type, lost_health, damage,
                                                 target->getResistantTo() == damage_type ? 50 : 100, armor_value,
                                                 target->isDead());
          shared_ptr<Player> attacked_player = dynamic_pointer_cast<Player>(target);
          shared_ptr<Enemy> attacked_enemy = dynamic_pointer_cast<Enemy>(target);
          if (attacked_player != nullptr)
          {
            attacked_field.setName(attacked_player->getName());
          }
          else if (attacked_enemy != nullptr)
          {
            string enemy_name = attacked_enemy->getTypeName() + " " + std::to_string(attacked_enemy->getId());
            attacked_field.setName(enemy_name);
            if (attacked_enemy->isBoss() && attacked_enemy->isDead())
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

vector<AttackedField> Dungeon::simulateAttack(shared_ptr<Character> attacker, int damage, pair<int, int> target_field)
{
  vector<AttackedField> attacked_fields;
  pair<int, int> attacker_position = current_room_->getFieldOfEntity(attacker);
  shared_ptr<Weapon> weapon;
  if (dynamic_pointer_cast<Player>(attacker) != nullptr)
  {
    shared_ptr<Player> player = dynamic_pointer_cast<Player>(attacker);
    weapon = player->getWeapon();
  }
  else
  {
    weapon = attacker->getWeapon();
  }
  DamageType damage_type = weapon->getDamageType();
  vector<vector<int>> affected_fields = weapon->getDamagePattern()->getAffectedFields(
          attacker_position, target_field, current_room_->getWidth(), current_room_->getHeight());
  for (size_t row_index = 0; row_index < affected_fields.size(); row_index++)
  {
    for (size_t column_index = 0; column_index < affected_fields[row_index].size(); column_index++)
    {
      if (affected_fields[row_index][column_index] == 1)
      {
        shared_ptr<Character> target =
                dynamic_pointer_cast<Character>(current_room_->getField({row_index + 1, column_index + 1})->getEntity());
        AttackedField attacked_field = AttackedField({row_index + 1, column_index + 1});
        if (target != nullptr && !boss_dead_ && !playersInRoomAreDead())
        {
          int lost_health = target->simulateDamage(damage, damage_type);
          if (target->isDead()) { attacked_field.setDead(true); }
          int armor_value = target->getBaseArmor();
          if (target->getArmor() != nullptr)
          {
            armor_value = target->getArmor()->getArmorValue();
            if (target->getArmor()->getAbbreviation() == "LARM")
            {
              armor_value = 1 + target->getVitality();
            }
            else if (target->getArmor()->getAbbreviation() == "BPLT")
            {
              armor_value = 4 + std::min(target->getVitality(), 2);
            }
          }
          shared_ptr<Player> target_player = dynamic_pointer_cast<Player>(target);
          shared_ptr<Enemy> target_enemy = dynamic_pointer_cast<Enemy>(target);
          CharacterType character_type;
          if (target_player != nullptr)
          {
            character_type = CharacterType::PLAYER;
          }
          else if (target_enemy != nullptr)
          {
            character_type = CharacterType::ENEMY;
          }
          else
          {
            character_type = CharacterType::NONE;
          }
          attacked_field.setCharacterWithoutName(character_type, lost_health, damage,
                                                 target->getResistantTo() == damage_type ? 50 : 100, armor_value,
                                                 target->isDead());
          shared_ptr<Player> attacked_player = dynamic_pointer_cast<Player>(target);
          shared_ptr<Enemy> attacked_enemy = dynamic_pointer_cast<Enemy>(target);
          if (attacked_player != nullptr)
          {
            attacked_field.setName(attacked_player->getName());
          }
          else if (attacked_enemy != nullptr)
          {
            string enemy_name = attacked_enemy->getTypeName() + " " + std::to_string(attacked_enemy->getId());
            attacked_field.setName(enemy_name);
            if (attacked_enemy->isBoss() && attacked_enemy->isDead())
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

void Dungeon::lootEntity(shared_ptr<Player> player, shared_ptr<Entity> entity)
{
  map<string, int> loot = entity->getLoot();
  int ret = player->getInventory()->parseInventory(loot, player->getAbbreviation(), player->getStrength(),
                                                   player->getVitality());
  if (ret == 1) { std::cout << "The entity contains an unknown item. The loot could not be parsed." << std::endl; }
  pair<int, int> position = getCurrentRoom()->getFieldOfEntity(entity);
  getCurrentRoom()->setFieldEntity(nullptr, position.first, position.second);
}

void Dungeon::killCharacter(shared_ptr<Character> character)
{
  pair<int, int> position = getCurrentRoom()->getFieldOfEntity(character);
  getCurrentRoom()->setFieldEntity(nullptr, position.first, position.second);
  map<string, int> loot = character->getInventory()->getInventoryMapped();
  shared_ptr<DeathLocation> death_location = make_shared<DeathLocation>(loot);
  getCurrentRoom()->setFieldEntity(death_location, position.first, position.second);
  shared_ptr<Player> player = dynamic_pointer_cast<Player>(character);
  if (player != nullptr) { player->kill(); }
}

void Dungeon::getBestAttack(std::shared_ptr<Player> player, int damage, std::pair<int, int>& lowest_hp_enemy_position,
                            std::vector<AttackedField>& attacked_fields, bool is_distant_line_or_burst)
{
    std::vector<std::tuple<std::vector<AttackedField>, int, std::pair<int, int>>> possible_attacks;

    std::vector<std::pair<int, int>> offsets = {{0,  0},
                                                {-1, 0},
                                                {1,  0},
                                                {0,  -1},
                                                {0,  1},
                                                {-1, -1},
                                                {-1, 1},
                                                {1,  -1},
                                                {1,  1}};

    for (const auto &offset: offsets)
    {
        int new_x = lowest_hp_enemy_position.first + offset.first;
        int new_y = lowest_hp_enemy_position.second + offset.second;
        std::pair<int,int> new_target_position = std::make_pair(new_x, new_y);
        int score = 0;
        std::vector<AttackedField> attack;

        if (new_target_position.first < 1 ||
            new_target_position.second < 1 ||
            static_cast<std::vector<int>::size_type>(new_target_position.first) > getCurrentRoom()->getFields().size() ||
            static_cast<std::vector<int>::size_type>(new_target_position.second) > getCurrentRoom()->getFields()[0].size())
        {
          continue;
        }

        try
        {
          attack = simulateAttack(player, damage, new_target_position);
        }
        catch(const std::out_of_range& e)
        {
          continue;
        }
        catch(const std::exception& e)
        {
          continue;
        }

        bool add_to_attacks = false;
        for(const auto& field : attack)
        {
            if(field.getCharacterType() == CharacterType::PLAYER)
            {
                score -= field.getLostHealth() * 10; // Prioritize player health over enemy damage.
            }
            else if(field.getCharacterType() == CharacterType::ENEMY)
            {
              if (field.getName().find("Goblin") != std::string::npos) {
                  score += field.getLostHealth() * 1.3; // All my homies hate goblins
              }
              else
              {
                score += field.getLostHealth();
              }
            }

            if(field.getPosition() == lowest_hp_enemy_position) //if one field contains the initial target. We still want to focus on enemy with lowest HP
            {
              add_to_attacks = true;
            }
        }
        if(add_to_attacks && score > 0)
        {
          possible_attacks.push_back(std::make_tuple(attack, score, offset));
        }

        if(is_distant_line_or_burst)
        {
          for(const auto& field : attack)
          {
            if(field.containsCharacter() && field.getCharacterType() == CharacterType::ENEMY && score > 0)
            {
              possible_attacks.push_back(std::make_tuple(attack, score, offset));
            }
          }
        }
    }

    auto best_attack_it = std::max_element(possible_attacks.begin(), possible_attacks.end(),
                                           [](const auto& a, const auto& b) {
                                               return std::get<1>(a) < std::get<1>(b);
                                           });

    if (best_attack_it != possible_attacks.end())
    {
      attacked_fields = std::get<0>(*best_attack_it);
      std::pair<int, int> best_offset = std::get<2>(*best_attack_it);
      lowest_hp_enemy_position.first += best_offset.first;
      lowest_hp_enemy_position.second += best_offset.second;
    }
}

bool Dungeon::canAttackMelee(std::shared_ptr<Player> player)
{
  if(!getCurrentRoom()->isEnemyNearby(player))
  {
    if(player->getWeapon()->getDamagePattern()->getPattern() == Pattern::THRUST
              || player->getWeapon()->getDamagePattern()->getPattern() == Pattern::LINE)
    {
      int damage = player->getWeapon()->getDamageAddition() + (player->getWeapon()->getDice()->getType() / 2) * player->getWeapon()->getDice()->getAmount();
      std::vector<AttackedField> attacked_fields;
      std::pair<int,int> player_pos = getCurrentRoom()->getFieldOfEntity(player);
      getBestAttack(player, damage, player_pos, attacked_fields, true);
      if(!attacked_fields.empty())
      {
        return true;
      }
    }
  }
  return getCurrentRoom()->isEnemyNearby(player);
}

