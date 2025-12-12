#pragma once
#include "GameObject.h"
#include <vector>
#include <memory>

class Player {
private:
    int health;
    static const int MAX_INVENTORY = 10;
    std::vector<std::unique_ptr<GameObject>> inventory;  // Храним GameObject*

public:
    Player(int hp = 3);

    void takeDamage(int dmg);
    void heal(int amount);
    bool isAlive() const;
    int getHealth() const;

    bool addItem(std::unique_ptr<GameObject> item);
    void showInventory() const;
    bool hasItem(const std::string& name) const;
    bool isInventoryFull() const;
    int countKeyItems() const;

    // Использование предметов через полиморфизм
    void useItem(const std::string& name);
    void useItem(const std::string& name, int value);
    void useItem(const std::string& name, const std::string& target);

    GameObject* getItem(const std::string& name);
    int getInventorySize() const;

    // Статический метод
    static int getMaxInventorySize();
};