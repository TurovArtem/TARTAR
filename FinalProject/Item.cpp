#include "Item.h"

int Item::totalItems = 0;

Item::Item(const std::string& n, const std::string& d, bool key)
    : GameObject(n, d), isKey(key), used(false) {
    totalItems++;
}

void Item::inspect() const {
    std::cout << getName() << ": " << getDescription();
    if (isKey) std::cout << " (ключевой предмет)";
    std::cout << std::endl;
}

void Item::use() {
    if (!used) {
        std::cout << "[+] Вы использовали: " << getName() << std::endl;
        used = true;
    }
    else {
        std::cout << "[-] Предмет уже использован" << std::endl;
    }
}

void Item::use(int value) {
    if (!used) {
        std::cout << "[+] Вы использовали: " << getName()
            << " со значением " << value << std::endl;
        used = true;
    }
}

void Item::use(const std::string& target) {
    if (!used) {
        std::cout << "[+] Вы использовали: " << getName()
            << " на " << target << std::endl;
        used = true;
    }
}

bool Item::getIsKey() const { return isKey; }
bool Item::isUsed() const { return used; }
bool Item::isKeyItem() const { return isKey; }

int Item::getTotalItems() {
    return totalItems;
}