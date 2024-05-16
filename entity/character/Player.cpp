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
  std::shared_ptr<Weapon> weapon = inventory_->getWeapon(weapon_abbreviation);
  if (weapon != nullptr)
  {
    weapon_ = inventory_->getWeapon(weapon_abbreviation);
  }
  else
  {
    std::cout << "Weapon " << weapon_abbreviation << " not found in inventory.\n";
  }
}

void Player::setArmor(std::string armor_abbreviation)
{
  std::shared_ptr<Armor> armor = inventory_->getArmor(armor_abbreviation);
  if (armor != nullptr)
  {
    armor_ = inventory_->getArmor(armor_abbreviation);
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
      if (getHealth() > getMaximumHealth())
      {
        setHealth(getMaximumHealth());
      }
      int health_after = getHealth();
      simplePrintNoId();
      std::cout << "regenerates " << health_after-health_before << " health." << std::endl;
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
  }
}


int Player::getAttackDamage()
{
  //because of the order in which the command needs to be handled, I had to check for these 2 exceptions
  //already. Enemies have infinite ammo. I don't say remove it, maybe just something to keep in mind.
  //btw I took the check ammunition lines from you for the exceptions, they are really elegant -Hanno

  // Returns -1 if no weapon is equipped, -2 if no ammunition is available
  if (getActiveWeapon() == nullptr)
  {
    return -1;
  }

  if (getActiveWeapon()->getAttackType() != AttackType::MELEE)
  {
    std::string ammoType = (getActiveWeapon()->getAbbreviation() == "SBOW" ||
                            getActiveWeapon()->getAbbreviation() == "LBOW") ? "ARRW" : "BOLT";
    if (getInventory()->getAmmunition(ammoType) == nullptr || getInventory()->getAmmunition(ammoType)->getAmount() == 0)
    {
      return -2;
    }
  }

  return getActiveWeapon()->getDice()->roll();
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
  }

  int defense_points = getBaseArmor() + additional_armor;
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

  std::cout << "on "<< "(" << position.first << "," << position.second << ")" << std::endl;
  if (single_line)
  {
    return;
  }
  const int name_width = 17;
  const int value_width = 6;

  std::cout << std::setw(name_width) << std::left << "  Armor Value:" << std::setw(value_width)
            << std::right << getBaseArmor() << "\n"
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

