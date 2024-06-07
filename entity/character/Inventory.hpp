//---------------------------------------------------------------------------------------------------------------------
//
// The Inventory class can represent the items of a character or loot of enemies and objects.
// It contains vectors for potions, weapons, armor and ammunition, the reason being to keep the items separated.
//
// Group: 086
//
// Author: Benjamin Demetz, 12320035
//---------------------------------------------------------------------------------------------------------------------
//

#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>
#include "../../items/Potion.hpp"
#include "../../items/Weapon.hpp"
#include "../../items/Armor.hpp"
#include "../../items/Ammunition.hpp"

class Inventory
{
  // The inventory is separated into vectors for each item type.
  std::vector<std::shared_ptr<Potion>> potions_;
  std::vector<std::shared_ptr<Weapon>> weapons_;
  std::vector<std::shared_ptr<Armor>> armor_;
  std::vector<std::shared_ptr<Ammunition>> ammunition_;

  public:
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Default constructor for the Inventory class
    //
    Inventory() = default;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Constructor for the Inventory class. You can pass a map of items and their quantities to it. They will then all
    /// be created based on the character stats and added to the inventory.
    ///
    /// @param inventory a map containing the items and their quantities
    /// @param player_abbreviation the abbreviation of the player
    /// @param player_strength the strength of the player
    /// @param player_vitality the vitality of the player
    //
    explicit Inventory(std::map<std::string, int>& inventory, char player_abbreviation,
                       int player_strength, int player_vitality);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to parse a map of items and their quantities and add them to the inventory.
    ///
    /// @param inventory a map containing the items and their quantities
    /// @param player_abbreviation the abbreviation of the player
    /// @param player_strength the strength of the player
    /// @param player_vitality the vitality of the player
    ///
    /// @return 0 if the parsing was successful, 1 otherwise
    //
    int parseInventory(std::map<std::string, int>& inventory, char player_abbreviation,
                       int player_strength, int player_vitality);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to add a potion to the inventory
    ///
    /// @param potion the potion to be added
    //
    void addPotion(std::shared_ptr<Potion> potion);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to add a weapon to the inventory
    ///
    /// @param weapon the weapon to be added
    //
    void addWeapon(std::shared_ptr<Weapon> weapon);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to add an armor to the inventory
    ///
    /// @param armor the armor to be added
    //
    void addArmor(std::shared_ptr<Armor> armor);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to add an ammunition to the inventory. Will merge the ammunition with the same abbreviation.
    /// Then delete the original one and only increase the amount of the existing one.
    ///
    /// @param ammunition the ammunition to be added
    //
    void addAmmunition(std::shared_ptr<Ammunition> ammunition);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to remove an item from the inventory
    ///
    /// @param item the item to be removed
    ///
    /// @return 0 if the item was removed successfully, 1 otherwise
    int removeItem(std::shared_ptr<Item> item);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Uses up a given ammunition if it exists in the inventory
    ///
    /// @param abbreviation the abbreviation of the ammunition to be used
    ///
    /// @return the amount of ammunition used, or -1 if the ammunition does not exist
    int useAmmunition(std::string& abbreviation);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get a potion from the inventory based on its abbreviation
    ///
    /// @param abbreviation the abbreviation of the potion
    ///
    /// @return a shared pointer to the potion
    //
    std::shared_ptr<Potion> getPotion(const std::string& abbreviation);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get a weapon from the inventory based on its abbreviation
    ///
    /// @param abbreviation the abbreviation of the weapon
    ///
    /// @return a shared pointer to the weapon
    //
    std::shared_ptr<Weapon> getWeapon(const std::string& abbreviation);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get a random weapon from the inventory
    ///
    /// @return a shared pointer to the weapon
    //
    std::shared_ptr<Weapon> getRandomWeapon();
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get a random weapon from the inventory based on its attack type
    ///
    /// @param attack_type the attack type of the weapon
    ///
    /// @return a shared pointer to the weapon
    //
    std::shared_ptr<Weapon> getRandomWeapon(AttackType attack_type);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get an armor from the inventory based on its abbreviation
    ///
    /// @param abbreviation the abbreviation of the armor
    ///
    /// @return a shared pointer to the armor
    //
    std::shared_ptr<Armor> getArmor(const std::string& abbreviation);
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get an ammunition from the inventory based on its abbreviation
    ///
    /// @param abbreviation the abbreviation of the ammunition
    ///
    /// @return a shared pointer to the ammunition
    //
    std::shared_ptr<Ammunition> getAmmunition(const std::string& abbreviation) const;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get all weapons from the inventory
    ///
    /// @return a vector containing all weapons
    //
    std::vector<std::shared_ptr<Weapon>> getAllWeapons() const {return weapons_;};
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get all armor from the inventory
    ///
    /// @return a vector containing all armor
    //
    std::vector<std::shared_ptr<Armor>> getAllArmor() const {return armor_;};
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get all ammunition from the inventory
    ///
    /// @return a vector containing all ammunition
    //
    std::vector<std::shared_ptr<Ammunition>> getAllAmmunition() const {return ammunition_;};
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get all potions from the inventory
    ///
    /// @return a vector containing all potions
    //
    std::vector<std::shared_ptr<Potion>> getAllPotions() const {return potions_;};
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Function to get the inventory mapped to a string and its quantity. When a player dies, its items are stored as
    /// a map and then when looted are recreated with the stats of the player.
    ///
    /// @return a map containing the items and their quantities
    //
    std::map<std::string, int> getInventoryMapped() const;

    bool containsRangeWeaponWithAmmunition() const;

    bool containsMeleeWeapon() const;

    bool hasAmmunitionForWeapon(std::shared_ptr<Weapon> weapon) const;

    std::vector<std::shared_ptr<Weapon>> getAllRangeWeaponWithAmmunition() const;

    std::shared_ptr<Weapon> getBestMeleeWeapon() const;

    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Copy constructor for the Inventory class
    ///
    /// @param other the inventory to be copied
    //
    Inventory(const Inventory& other) = default;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Assignment operator for the Inventory class. Deleted because the inventory should not be copied.
    ///
    /// @param other the inventory to be assigned
    ///
    Inventory& operator=(const Inventory& other) = delete;
    ///----------------------------------------------------------------------------------------------------------------
    ///
    /// Destructor for the Inventory class
    //
    ~Inventory() = default;
};

#endif //INVENTORY_HPP
