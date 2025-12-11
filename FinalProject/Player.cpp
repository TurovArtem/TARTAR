#include "Player.h"
#include <iostream>
#include <algorithm>

Player::Player(int hp) : health(hp) {}

void Player::takeDamage(int dmg) {
    health -= dmg;
    if (health < 0) health = 0;
    std::cout << "[-] Вы получили урон. Здоровье: " << health << "/3" << std::endl;
}

bool Player::isAlive() const { return health > 0; }
int Player::getHealth() const { return health; }

bool Player::addItem(std::unique_ptr<Item> item) {
    if (isInventoryFull()) {
        std::cout << "[-] Инвентарь полон!" << std::endl;
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

    std::cout << "\n=== ИНВЕНТАРЬ ===" << std::endl;
    for (const auto& item : inventory) {
        std::cout << "- ";
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
    return inventory.size() >= MAX_INVENTORY;
}

int Player::countKeyItems() const {
    int count = 0;
    for (const auto& item : inventory) {
        if (item->getIsKey()) count++;
    }
    return count;
}