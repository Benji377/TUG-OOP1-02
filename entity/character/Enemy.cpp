#include "Enemy.hpp"
#include "../../utility/Props.hpp"

Enemy::Enemy(int id, char abbreviation): Character(id, abbreviation)
{
  switch (abbreviation)
  {
    case 'Z':
      type_name_ = "Zombie";
      is_boss_ = false;
      resistant_to_ = Effect::NONE;
      base_armor_ = 1;
      strength_ = 3;
      vitality_ = 0;
      maximum_health_ = 15;
      health_ = maximum_health_;
      break;
    case 'G':
      type_name_ = "Goblin";
      is_boss_ = false;
      resistant_to_ = Effect::NONE;
      base_armor_ = 4;
      strength_ = 3;
      vitality_ = 4;
      maximum_health_ = 10;
      health_ = maximum_health_;
      break;
    case 'L':
      type_name_ = "Lich";
      is_boss_ = true;
      resistant_to_ = Effect::COLD;
      base_armor_ = 6;
      strength_ = 2;
      vitality_ = 8;
      maximum_health_ = 50;
      health_ = maximum_health_;
      break;
    default:
      throw std::invalid_argument("Invalid abbreviation for enemy character.");
  }
}

void Enemy::initializeInventory()
{
  if (abbreviation_ == 'Z')
  {
    weapon_ = Props::craftWeapon((std::string &) "DAGG", strength_, vitality_);
    armor_ = nullptr;
    loot_ = {{"DAGG", 1}};
    inventory_ = nullptr;
  }
  else if (abbreviation_ == 'G')
  {
    weapon_ = Props::craftWeapon((std::string &) "HAXE", strength_, vitality_);
    armor_ = nullptr;
    inventory_ = new Inventory();
    inventory_->addWeapon(weapon_);
    inventory_->addWeapon(Props::craftWeapon((std::string &) "SBOW", strength_, vitality_));
  }
  else if (abbreviation_ == 'L')
  {
    weapon_ = Props::craftWeapon((std::string &) "QFIR", 'L');
    armor_ = nullptr;
    inventory_ = new Inventory();
    inventory_->addWeapon(weapon_);
    inventory_->addWeapon(Props::craftWeapon((std::string &) "QLCD", 'L'));
    inventory_->addWeapon(Props::craftWeapon((std::string &) "QACD", 'L'));
    inventory_->addWeapon(Props::craftWeapon((std::string &) "QFRC", 'L'));
  }
}


// Sorry muss san um di errors zi fixen
void Enemy::attack(Character& target, int damage) { }
void Enemy::take_damage(int damage) { }
int Enemy::move(int row, int column) { return 0; }