#include "Player.hpp"
#include <iomanip>
#include <utility>
#include <algorithm>
#include "../../utility/Props.hpp"
#include "../../utility/InputOutput.hpp"

Player::Player(int id, char abbreviation, std::string name) : Character(id, abbreviation)
{
  name_ = std::move(name);
  switch (abbreviation)
  {
    case 'B':
      type_name_ = "Barbarian";
      resistant_to_ = DamageType::NONE;
      base_armor_ = 2;
      strength_ = 4;
      vitality_ = 1;
      maximum_health_ = 12 + vitality_;
      health_ = maximum_health_;
      break;
    case 'R':
      type_name_ = "Rogue";
      resistant_to_ = DamageType::NONE;
      base_armor_ = 1;
      strength_ = 2;
      vitality_ = 3;
      maximum_health_ = 8 + vitality_;
      health_ = maximum_health_;
      break;
    case 'W':
      type_name_ = "Wizard";
      resistant_to_ = DamageType::NONE;
      base_armor_ = 0;
      strength_ = 1;
      vitality_ = 4;
      maximum_health_ = 6 + vitality_;
      health_ = maximum_health_;
      break;
    default:
      throw std::invalid_argument("Invalid abbreviation for player character.");
  }
  initializeInventory();
}

void Player::initializeInventory()
{
  std::shared_ptr<Inventory> temp_inv = std::make_shared<Inventory>(); // Create a shared pointer to Inventory

  if (getAbbreviation() == 'B')
  {
    weapon_ = Props::craftWeapon("GAXE", getAbbreviation(), getStrength(), getVitality());
    armor_ = nullptr;
    temp_inv->addWeapon(weapon_);
    temp_inv->addWeapon(Props::craftWeapon("HAXE", getAbbreviation(), getStrength(), getVitality()));
    temp_inv->addWeapon(Props::craftWeapon("HAXE", getAbbreviation(), getStrength(), getVitality()));
  }
  else if (getAbbreviation() == 'R')
  {
    weapon_ = Props::craftWeapon("RAPI", getAbbreviation(), getStrength(), getVitality());
    armor_ = Props::craftArmor("LARM", getVitality());

    temp_inv->addArmor(armor_);
    temp_inv->addWeapon(weapon_);
    temp_inv->addWeapon(Props::craftWeapon("DAGG", getAbbreviation(), getStrength(), getVitality()));
    temp_inv->addWeapon(Props::craftWeapon("DAGG", getAbbreviation(), getStrength(), getVitality()));
    temp_inv->addWeapon(Props::craftWeapon("SBOW", getAbbreviation(), getStrength(), getVitality()));
    temp_inv->addAmmunition(Props::craftAmmunition("ARRW", 20));
  }
  else if (getAbbreviation() == 'W')
  {
    weapon_ = Props::craftWeapon("QFRC", 'W');
    armor_ = nullptr;
    temp_inv->addWeapon(weapon_);
    temp_inv->addWeapon(Props::craftWeapon("QACD", getAbbreviation(), getStrength(), getVitality()));
    temp_inv->addWeapon(Props::craftWeapon("DAGG", getAbbreviation(), getStrength(), getVitality()));
  }

  inventory_ = temp_inv;
}

// Getter and setter methods

void Player::setResistance(DamageType damage_type)
{
  resistant_to_ = damage_type;
}

void Player::setActiveWeapon(std::string weapon_abbreviation)
{
  if (weapon_abbreviation.empty())
  {
    weapon_ = nullptr;
    return;
  }

  std::shared_ptr<Weapon> weapon = inventory_->getWeapon(weapon_abbreviation);
  if (weapon != nullptr)
  {
    weapon_ = weapon;
  }
  else
  {
    std::cout << "Weapon " << weapon_abbreviation << " not found in inventory.\n";
  }
}

void Player::setArmor(std::string armor_abbreviation)
{
  if (armor_abbreviation.empty())
  {
    armor_ = nullptr;
    return;
  }

  std::shared_ptr<Armor> armor = inventory_->getArmor(armor_abbreviation);
  if (armor != nullptr)
  {
    armor_ = armor;
  }
  else
  {
    std::cout << "Armor " << armor_abbreviation << " not found in inventory.\n";
  }
}

void Player::simplePrintNoId() const
{
  std::cout << this->getTypeName() << " " << "[" << this->getAbbreviation() << "] \"" <<
            this->getName() << "\"";
}


int Player::usePotion(std::string abbreviation)
{
  std::shared_ptr<Potion> potion = getInventory()->getPotion(abbreviation);
  if (potion == nullptr)
  {
    return 1;
  }
  else
  {
    if (potion->getEffect() == Effect::HEALTH)
    {
      int health_before = getHealth();
      setHealth(getHealth() + potion->getDice()->roll());
      InputOutput::printDiceRoll(potion->getDice()->getPreviousRoll(), potion->getDice());
      if (getHealth() > getMaximumHealth())
      {
        setHealth(getMaximumHealth());
      }
      int health_after = getHealth();
      simplePrintNoId();
      std::cout << " regenerates " << health_after - health_before << " health." << std::endl;
    }
    else
    {
      // Map the potion effect enum to the weapons damage type enum
      auto damage_type = static_cast<DamageType>(potion->getEffect());
      setResistance(damage_type);
      simplePrintNoId();
      InputOutput::printDamageTypeResistance(damage_type);
    }
    getInventory()->removeItem(potion);
    return 0;
  }
}


int Player::getAttackDamage()
{
  // Returns -1 if no weapon is equipped, -2 if no ammunition is available
  if (getWeapon() == nullptr)
  {
    return -1;
  }

  if (getWeapon()->getAttackType() != AttackType::MELEE //Ranged Quarterstaffs don't need ammo
      && getWeapon()->getAbbreviation() != "QFIR" && getWeapon()->getAbbreviation() != "QACD")
  {
    std::string ammo_type = (getWeapon()->getAbbreviation() == "SBOW" ||
                            getWeapon()->getAbbreviation() == "LBOW") ? "ARRW" : "BOLT";

    if (getInventory()->getAmmunition(ammo_type) == nullptr ||
        getInventory()->getAmmunition(ammo_type)->getAmount() == 0)
    {
      return -2;
    }
    getInventory()->useAmmunition(ammo_type);
  }

  return getWeapon()->getDamage();
}

int Player::takeDamage(int damage, DamageType damage_type)
{
  if (getResistantTo() == damage_type)
  {
    damage /= 2;
  }

  int additional_armor = 0;
  if (getArmor() != nullptr)
  {
    additional_armor = getArmor()->getArmorValue();
  }

  int defense_points = std::max(getBaseArmor(), additional_armor);
  int damage_taken = damage - defense_points;
  int lost_health = std::min(getHealth(), std::max(0, damage_taken));
  setHealth(getHealth() - lost_health);

  if (getHealth() <= 0)
  {
    kill();
  }
  return lost_health;
}

int Player::simulateDamage(int damage, DamageType damage_type)
{
  if (getResistantTo() == damage_type)
  {
    damage /= 2;
  }

  int additional_armor = 0;
  if (getArmor() != nullptr)
  {
    additional_armor = getArmor()->getArmorValue();
  }

  int defense_points = std::max(getBaseArmor(), additional_armor);
  int damage_taken = damage - defense_points;
  int lost_health = std::min(getHealth(), std::max(0, damage_taken));
  return lost_health;
}

void Player::printPlayer(const std::pair<int, int> &position, bool single_line) const
{
  simplePrintNoId();

  std::cout << " on " << "(" << position.first << "," << position.second << ")" << std::endl;
  if (single_line)
  {
    return;
  }
  const int NAME_WIDTH = 17;
  const int VALUE_WIDTH = 6;
  const int ARMOR_VALUE = std::max(getBaseArmor(), getArmor() ? getArmor()->getArmorValue() : 0);

  std::cout << std::setw(NAME_WIDTH) << std::left << "  Armor Value:" << std::setw(VALUE_WIDTH)
            << std::right << ARMOR_VALUE << "\n"
            << std::setw(NAME_WIDTH) << std::left << "  Current Health:" << std::setw(VALUE_WIDTH)
            << std::right << getHealth() << "\n"
            << std::setw(NAME_WIDTH) << std::left << "  Max Health:" << std::setw(VALUE_WIDTH)
            << std::right << getMaximumHealth() << "\n"
            << std::setw(NAME_WIDTH) << std::left << "  Strength:" << std::setw(VALUE_WIDTH)
            << std::right << getStrength() << "\n"
            << std::setw(NAME_WIDTH) << std::left << "  Vitality:" << std::setw(VALUE_WIDTH)
            << std::right << getVitality() << std::endl;
}

void Player::simplePrint() const
{
  std::cout << "Player " << getId() << ": " << getTypeName()
            << " [" << getAbbreviation() << "] \"" << getName() << "\"" << std::endl;
}

std::shared_ptr<Weapon> Player::getBestRangeWeaponWithAmmunition() const
{
  std::shared_ptr<Weapon> best_range_weapon_with_ammo;
  std::vector<std::shared_ptr<Weapon>> ranged_weapons_with_ammo = inventory_->getAllRangeWeaponWithAmmunition();
  if (getWeapon() != nullptr && getWeapon()->getAttackType() == AttackType::RANGED && getInventory()->hasAmmunitionForWeapon(getWeapon()))
  {
    best_range_weapon_with_ammo = getWeapon();
  }
  for (auto &weapon : ranged_weapons_with_ammo)
  {
    if (best_range_weapon_with_ammo == nullptr || weapon->getHighestDamage() > best_range_weapon_with_ammo->getHighestDamage())
    {
      best_range_weapon_with_ammo = weapon;
    }
  }
  return best_range_weapon_with_ammo;
}

std::shared_ptr<Weapon> Player::getBestMeleeWeapon() const
{
  return inventory_->getBestMeleeWeapon();
  return nullptr;
}

bool Player::hasRangeWeaponWithAmmunition() const
{
  std::shared_ptr<Weapon> equipped_weapon = getWeapon();
  if (equipped_weapon != nullptr && equipped_weapon->getAttackType() == AttackType::RANGED && getInventory()->hasAmmunitionForWeapon(equipped_weapon))
  {
    return true;
  }
  else
  {
    return getInventory()->containsRangeWeaponWithAmmunition();
  }
  return false;
}

bool Player::hasRangeWeaponEquipped() const
{
  return getWeapon() != nullptr && getWeapon()->getAttackType() == AttackType::RANGED;
}

bool Player::hasMeleeWeapon() const
{
  if (getWeapon() != nullptr && getWeapon()->getAttackType() == AttackType::MELEE)
  {
    return true;
  }
  else
  {
    return getInventory()->containsMeleeWeapon();
  }
  return false;
}

bool Player::hasMeleeWeaponEquipped() const
{
  return getWeapon() != nullptr && getWeapon()->getAttackType() == AttackType::MELEE;
}

bool Player::hasBetterArmor() const
{
  return getArmor() != getBestArmor();
}

std::shared_ptr<Armor> Player::getBestArmor() const
{
  std::shared_ptr<Armor> best_armor = getArmor();
  std::vector<std::shared_ptr<Armor>> armors = inventory_->getAllArmor();
  for (auto &armor : armors)
  {
    if (best_armor == nullptr || armor->getArmorValue() > best_armor->getArmorValue())
    {
      best_armor = armor;
    }
  }
  return best_armor;
}