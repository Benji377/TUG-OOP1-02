#include "Enemy.hpp"
#include "../../utility/Props.hpp"

Enemy::Enemy(int id, char abbreviation): Character(id, abbreviation)
{
  switch (abbreviation)
  {
    case 'Z':
      type_name_ = "Zombie";
      is_boss_ = false;
      resistant_to_ = DamageType::NONE;
      base_armor_ = 1;
      strength_ = 3;
      vitality_ = 0;
      maximum_health_ = 15;
      health_ = maximum_health_;
      break;
    case 'G':
      type_name_ = "Goblin";
      is_boss_ = false;
      resistant_to_ = DamageType::NONE;
      base_armor_ = 4;
      strength_ = 3;
      vitality_ = 4;
      maximum_health_ = 10;
      health_ = maximum_health_;
      break;
    case 'L':
      type_name_ = "Lich";
      is_boss_ = true;
      resistant_to_ = DamageType::COLD;
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
    inventory_ = std::make_shared<Inventory>();
    inventory_->addWeapon(weapon_);
    inventory_->addWeapon(Props::craftWeapon("SBOW", strength_, vitality_));
  }
  else if (abbreviation_ == 'L')
  {
    weapon_ = Props::craftWeapon("QFIR", 'L');
    armor_ = nullptr;
    inventory_ = std::make_shared<Inventory>();
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

// TODO: Enemy can only use melee if the player is nearby
// Therefore we need to reroll or something
int Enemy::getAttackDamage()
{
  if (getInventory() != nullptr)
  {
    setWeapon(getInventory()->getRandomWeapon());
    return getWeapon()->getDice()->roll();
  }
  return -1;
}

int Enemy::takeDamage(int damage, DamageType damage_type)
{
  if (getResistantTo() == damage_type)
  {
    damage /= 2;
  }
  int defense_points = getBaseArmor() + getArmor()->getArmorValue();
  int damage_taken = damage - defense_points;
  int lost_health = std::min(getHealth(), std::max(0, damage_taken));
  setHealth(getHealth() - lost_health);

  if (getHealth() <= 0)
  {
    kill();
  }
  return lost_health;
}

void Enemy::printEnemy(const std::string& id_string, const std::pair<int, int>& position) const
{
  std::cout << id_string << " [" << getAbbreviation() << getId() << "] on (" << position.first
            << ","  << position.second << ")" << std::endl;
}

void Enemy::simplePrint() const
{
  std::cout << getAbbreviation() << getId() << ": " << getHealth() << "/" << getMaximumHealth();
}