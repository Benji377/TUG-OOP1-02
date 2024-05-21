#include "Player.hpp"
#include <iomanip>
#include <utility>
#include "../../utility/Props.hpp"
#include "../../utility/IO.hpp"

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
    weapon_ = Props::craftWeapon("GAXE", getStrength(), getVitality());
    armor_ = nullptr;
    temp_inv->addWeapon(weapon_);
    temp_inv->addWeapon(Props::craftWeapon("HAXE", getStrength(), getVitality()));
    temp_inv->addWeapon(Props::craftWeapon("HAXE", getStrength(), getVitality()));
  }
  else if (getAbbreviation() == 'R')
  {
    weapon_ = Props::craftWeapon("RAPI", getStrength(), getVitality());
    armor_ = Props::craftArmor("LARM", getVitality());

    temp_inv->addArmor(armor_);
    temp_inv->addWeapon(weapon_);
    temp_inv->addWeapon(Props::craftWeapon("DAGG", getStrength(), getVitality()));
    temp_inv->addWeapon(Props::craftWeapon("DAGG", getStrength(), getVitality()));
    temp_inv->addWeapon(Props::craftWeapon("SBOW", getStrength(), getVitality()));
    temp_inv->addAmmunition(Props::craftAmmunition("ARRW", 20));
  }
  else if (getAbbreviation() == 'W')
  {
    weapon_ = Props::craftWeapon("QFRC", 'W');
    armor_ = nullptr;
    temp_inv->addWeapon(weapon_);
    temp_inv->addWeapon(Props::craftWeapon("QACD", 'W'));
    temp_inv->addWeapon(Props::craftWeapon("DAGG", getStrength(), getVitality()));
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

std::shared_ptr<Weapon> Player::getActiveWeapon() const
{
  return weapon_;
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
      IO::printDiceRoll(potion->getDice()->getPreviousRoll(), potion->getDice());
      if (getHealth() > getMaximumHealth())
      {
        setHealth(getMaximumHealth());
      }
      int health_after = getHealth();
      simplePrintNoId();
      std::cout << " regenerates " << health_after-health_before << " health." << std::endl;
    }
    else
    {
      // Map the potion effect enum to the weapons damage type enum
      auto damage_type = static_cast<DamageType>(potion->getEffect());
      // TODO: This cast may not work properly!
      setResistance(damage_type);
      simplePrintNoId();
      IO::printDamageTypeResistance(damage_type);
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
    std::string ammoType = (getWeapon()->getAbbreviation() == "SBOW" ||
                            getWeapon()->getAbbreviation() == "LBOW") ? "ARRW" : "BOLT";

    if (getInventory()->getAmmunition(ammoType) == nullptr || getInventory()->getAmmunition(ammoType)->getAmount() == 0)
    {
      return -2;
    }
    getInventory()->useAmmunition(ammoType);
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
  if(getArmor() != nullptr)
  {
        additional_armor = getArmor()->getArmorValue();

    if(getArmor()->getAbbreviation() == "LARM") //TODO do enemies have armor??
    {
      additional_armor = 1 + getVitality();
    }
    else if(getArmor()->getAbbreviation() == "BPLT")
    {
      additional_armor = 4 + std::min(getVitality(), 2);
    }
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

void Player::printPlayer(const std::pair<int, int>& position, bool single_line) const
{
  simplePrintNoId();

  std::cout << " on "<< "(" << position.first << "," << position.second << ")" << std::endl;
  if (single_line)
  {
    return;
  }
  const int name_width = 17;
  const int value_width = 6;
  const int armor_value = std::max(getBaseArmor(), getArmor() ? getArmor()->getArmorValue() : 0);

  std::cout << std::setw(name_width) << std::left << "  Armor Value:" << std::setw(value_width)
            << std::right << armor_value << "\n"
            << std::setw(name_width) << std::left << "  Current Health:" << std::setw(value_width)
            << std::right << getHealth() << "\n"
            << std::setw(name_width) << std::left << "  Max Health:" << std::setw(value_width)
            << std::right << getMaximumHealth() << "\n"
            << std::setw(name_width) << std::left << "  Strength:" << std::setw(value_width)
            << std::right << getStrength() << "\n"
            << std::setw(name_width) << std::left << "  Vitality:" << std::setw(value_width)
            << std::right << getVitality() << std::endl;
}

void Player::simplePrint() const
{
  std::cout << "Player " << getId() << ": " << getTypeName()
            << " [" << getAbbreviation() << "] \"" << getName() << "\"" << std::endl;
}

std::shared_ptr<Weapon> Player::getWeapon() const
{
  if(weapon_ == nullptr)
  {
    return nullptr;
  }

  if(weapon_->getAbbreviation().compare(0, 1, "Q") == 0)
  {
    if(getAbbreviation() == 'W')
    {
      if(weapon_->getAbbreviation() == "QFIR")
      {
        weapon_->setAttackType(AttackType::RANGED);
        weapon_->setDamageType(DamageType::FIRE);

        std::string pattern_str = "burst";
        auto pattern = std::make_shared<DamagePattern>(pattern_str);
        weapon_->setDamangePattern(pattern);
      }
      else if(weapon_->getAbbreviation() == "QCLD")
      {
         weapon_->setDamageType(DamageType::COLD);

        std::string pattern_str = "line";
        auto pattern = std::make_shared<DamagePattern>(pattern_str);
        weapon_->setDamangePattern(pattern);
      }
      else if(weapon_->getAbbreviation() == "QACD")
      {
        weapon_->setAttackType(AttackType::RANGED);
        weapon_->setDamageType(DamageType::ACID);

        std::string pattern_str = "shot";
        auto pattern = std::make_shared<DamagePattern>(pattern_str);
        weapon_->setDamangePattern(pattern);
      }
      else if(weapon_->getAbbreviation() == "QFRC")
      {
        weapon_->setDamageType(DamageType::FORCE);
      }
    }
    else
    {
      weapon_->setAttackType(AttackType::MELEE);
      weapon_->setDamageType(DamageType::PHYSICAL);

      std::string pattern_str = "hit";
      auto pattern = std::make_shared<DamagePattern>(pattern_str);
      weapon_->setDamangePattern(pattern);
    }
  }

  return weapon_;

}
