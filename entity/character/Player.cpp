#include "Player.hpp"
#include <iomanip>
#include <utility>
#include "../../utility/Props.hpp"

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

DamageType Player::getResistance() const
{
  return resistant_to_;
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
      setHealth(getHealth() + potion->getDice()->roll());
      if (getHealth() > getMaximumHealth())
      {
        setHealth(getMaximumHealth());
      }
    }
    else
    {
      // Map the potion effect enum to the weapons damage type enum
      auto damage_type = static_cast<DamageType>(potion->getEffect());
      // TODO: This cast may not work properly!
      setResistance(damage_type);
    }
    getInventory()->removeItem(potion);
  }
}

// Sorry muss san um di errors zi fixen
void Player::attack(Character& target, int damage) { }

void Player::takeDamage(int damage, DamageType damageType)
{
  if (getResistance() == damageType)
  {
    return;
  }
  int defense_points = getBaseArmor() + getArmor()->getArmorValue();
  int damage_taken = damage - defense_points;
  if (damage_taken < 0)
  {
    damage_taken = 0;
  }
  setHealth(getHealth() - damage_taken);
}
int Player::move(int row, int column) { return 0; }


void Player::printPlayer(const std::pair<int, int>& position, bool single_line) const
{
  std::cout << getTypeName() << " [" << getAbbreviation() << "] \"" << getName()
            << "\" on (" << position.first << "," << position.second << ")" << std::endl;;
  if (single_line)
  {
    return;
  }

  const int name_width = 15;
  const int value_width = 5;

  std::cout << std::setw(15) << std::right << "Armor Value: " << std::setw(5) << std::right << getBaseArmor() << "\n"
      << std::setw(15) << std::right << "Current Health: " << std::setw(5) << std::right << getHealth() << "\n"
      << std::setw(15) << std::right << "Max Health: " << std::setw(5) << std::right << getMaximumHealth() << "\n"
      << std::setw(15) << std::right << "Strength: " << std::setw(5) << std::right << getStrength() << "\n"
      << std::setw(15) << std::right << "Vitality: " << std::setw(5) << std::right << getVitality() << std::endl;
}

void Player::simplePrint() const
{
  std::cout << "Player " << getId() << ": " << getTypeName()
            << " [" << getAbbreviation() << "] \"" << getName() << "\"" << std::endl;
}
