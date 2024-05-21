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
  if (getAbbreviation() == 'Z')
  {
    weapon_ = Props::craftWeapon("DAGG", getAbbreviation(), getStrength(), getVitality());
    armor_ = nullptr;
    loot_ = {{"DAGG", 1}};
    inventory_ = std::make_shared<Inventory>();
    getInventory()->addWeapon(weapon_);
  }
  else if (getAbbreviation() == 'G')
  {
    weapon_ = Props::craftWeapon("HAXE", getAbbreviation(), getStrength(), getVitality());
    armor_ = nullptr;
    loot_ = {{"HAXE", 1}, {"SBOW", 1}, {"ARRW", 3}};
    inventory_ = std::make_shared<Inventory>();
    getInventory()->addWeapon(weapon_);
    getInventory()->addWeapon(Props::craftWeapon("SBOW", getAbbreviation(), getStrength(), getVitality()));
  }
  else if (getAbbreviation() == 'L')
  {
    weapon_ = Props::craftWeapon("QFIR", getAbbreviation(), getStrength(), getVitality());
    armor_ = nullptr;
    inventory_ = std::make_shared<Inventory>();
    getInventory()->addWeapon(weapon_);
    getInventory()->addWeapon(Props::craftWeapon("QCLD", getAbbreviation(), getStrength(), getVitality()));
    getInventory()->addWeapon(Props::craftWeapon("QACD", getAbbreviation(), getStrength(), getVitality()));
    getInventory()->addWeapon(Props::craftWeapon("QFRC", getAbbreviation(), getStrength(), getVitality()));
  }
}

bool Enemy::is_boss() const
{
  return is_boss_;
}

int Enemy::getAttackDamage()
{
  if (getInventory() != nullptr)
  {
    setWeapon(getInventory()->getRandomWeapon());
    return getWeapon()->getDamage();
  }
  return -1;
}

int Enemy::getAttackDamage(AttackType attack_type)
{
  if (getInventory() != nullptr)
  {
    std::shared_ptr<Weapon> weapon = getInventory()->getRandomWeapon(attack_type);
    if (weapon == nullptr)
    {
      return -1;
    }
    setWeapon(weapon);
    return getWeapon()->getDamage();
  }
  return -1;
}

int Enemy::takeDamage(int damage, DamageType damage_type)
{
  if (getResistantTo() == damage_type)
  {
    damage /= 2;
  }

  int damage_taken = damage - getBaseArmor();
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

void Enemy::attackPrint() const
{
  std::cout << getTypeName() << " " << getId() << " [" << getAbbreviation() << getId() << "]";
}
