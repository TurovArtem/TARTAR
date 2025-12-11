#pragma once
#include <vector>
#include <memory>
#include "Item.h"

class Player {
private:
    int health;
    static const int MAX_INVENTORY = 5;
    std::vector<std::unique_ptr<Item>> inventory;

public:
    Player(int hp = 3);

    void takeDamage(int dmg);
    bool isAlive() const;
    int getHealth() const;

    bool addItem(std::unique_ptr<Item> item);
    void showInventory() const;
    bool hasItem(const std::string& name) const;
    bool isInventoryFull() const;
    int countKeyItems() const;
};