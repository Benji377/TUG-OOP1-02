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
  initializeInventory();
}

void Enemy::initializeInventory()
{
  if (abbreviation_ == 'Z')
  {
    weapon_ = Props::craftWeapon("DAGG", strength_, vitality_);
    armor_ = nullptr;
    loot_ = {{"DAGG", 1}};
    inventory_ = nullptr;
  }
  else if (abbreviation_ == 'G')
  {
    weapon_ = Props::craftWeapon("HAXE", strength_, vitality_);
    armor_ = nullptr;
    loot_ = {{"HAXE", 1}, {"SBOW", 1}, {"ARRW", 3}};
    inventory_ = new Inventory();
    inventory_->addWeapon(weapon_);
    inventory_->addWeapon(Props::craftWeapon("SBOW", strength_, vitality_));
  }
  else if (abbreviation_ == 'L')
  {
    weapon_ = Props::craftWeapon("QFIR", 'L');
    armor_ = nullptr;
    inventory_ = new Inventory();
    inventory_->addWeapon(weapon_);
    inventory_->addWeapon(Props::craftWeapon("QCLD", 'L'));
    inventory_->addWeapon(Props::craftWeapon("QACD", 'L'));
    inventory_->addWeapon(Props::craftWeapon("QFRC", 'L'));
  }
}

bool Enemy::is_boss() const
{
  return is_boss_;
}

// Sorry muss san um di errors zi fixen
void Enemy::attack(Character& target, int damage) { }
void Enemy::takeDamage(int damage) { }
int Enemy::move(int row, int column) { return 0; }

Enemy::~Enemy()
{
  delete inventory_;
  delete weapon_;
  delete armor_;
}

void Enemy::printEnemy() const
{
  std::cout << "Enemy: " << type_name_ << std::endl;
  std::cout << "Health: " << health_ << "/" << maximum_health_ << std::endl;
  std::cout << "Strength: " << strength_ << std::endl;
  std::cout << "Vitality: " << vitality_ << std::endl;
  std::cout << "Armor: " << base_armor_ << std::endl;
  std::cout << "Weapon: " << weapon_->getName() << std::endl;
}
