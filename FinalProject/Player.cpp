#include "Player.h"
#include "Item.h"
#include "Consumable.h"
#include <iostream>
#include <algorithm>

Player::Player(int hp) : health(hp) {}

void Player::takeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
    std::cout << "[-] Вы получили урон " << dmg << ". Здоровье: " << health << "/3" << std::endl;
}

void Player::heal(int amount) {
    health += amount;
    if (health > 3) health = 3;
    std::cout << "[+] Вы восстановили " << amount << " здоровья. Здоровье: " << health << "/3" << std::endl;
}

bool Player::isAlive() const { return health > 0; }
int Player::getHealth() const { return health; }

bool Player::addItem(std::unique_ptr<GameObject> item) {
    if (isInventoryFull()) {
        std::cout << "[-] Инвентарь полон! (" << getInventorySize()
            << "/" << MAX_INVENTORY << ")" << std::endl;
        return false;
    }
    inventory.push_back(std::move(item));
    std::cout << "[+] Вы взяли: " << inventory.back()->getName() << std::endl;
    return true;
}

void Player::showInventory() const {
    if (inventory.empty()) {
        std::cout << "Инвентарь пуст." << std::endl;
        return;
    }

    std::cout << "\n=== ИНВЕНТАРЬ (" << getInventorySize()
        << "/" << MAX_INVENTORY << ") ===" << std::endl;

    // Полиморфный вызов inspect() для каждого предмета
    for (const auto& item : inventory) {
        item->inspect();
    }

    std::cout << "Здоровье: " << health << "/3" << std::endl;
    std::cout << "=================" << std::endl;
}

bool Player::hasItem(const std::string& name) const {
    for (const auto& item : inventory) {
        if (item->getName() == name) return true;
    }
    return false;
}

bool Player::isInventoryFull() const {
    return getInventorySize() >= MAX_INVENTORY;
}

int Player::countKeyItems() const {
    int count = 0;
    for (const auto& item : inventory) {
        // Полиморфный вызов isKeyItem()
        if (item->isKeyItem()) {
            count++;
        }
    }
    return count;
}

void Player::useItem(const std::string& name) {
    for (auto& item : inventory) {
        if (item->getName() == name) {
            item->use();  // Полиморфный вызов
            return;
        }
    }
    std::cout << "У вас нет предмета: " << name << std::endl;
}

void Player::useItem(const std::string& name, int value) {
    for (auto& item : inventory) {
        if (item->getName() == name) {
            item->use(value);  // Полиморфный вызов перегруженного метода
            return;
        }
    }
    std::cout << "У вас нет предмета: " << name << std::endl;
}

void Player::useItem(const std::string& name, const std::string& target) {
    for (auto& item : inventory) {
        // Проверяем, является ли предмет наследником Item
        if (item->getName() == name) {
            // Динамическое приведение типа для доступа к методу use(target)
            if (Item* specificItem = dynamic_cast<Item*>(item.get())) {
                specificItem->use(target);
            }
            else {
                std::cout << "Этот предмет нельзя использовать таким образом." << std::endl;
            }
            return;
        }
    }
    std::cout << "У вас нет предмета: " << name << std::endl;
}

GameObject* Player::getItem(const std::string& name) {
    for (auto& item : inventory) {
        if (item->getName() == name) {
            return item.get();
        }
    }
    return nullptr;
}

int Player::getInventorySize() const {
    return static_cast<int>(inventory.size());
}

int Player::getMaxInventorySize() {
    return MAX_INVENTORY;
}