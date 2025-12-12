#include "Item.h"

int Item::totalItems = 0;

Item::Item(const std::string& n, const std::string& d, bool key)
    : name(n), description(d), isKey(key), used(false) {
    totalItems++;
}

void Item::inspect() const {
    std::cout << name << ": " << description;
    if (isKey) std::cout << " (ключевой)";
    std::cout << std::endl;
}

void Item::use() {
    if (!used) {
        std::cout << "[+] Вы использовали: " << name << std::endl;
        used = true;
    }
    else {
        std::cout << "[-] Предмет уже использован" << std::endl;
    }
}

void Item::use(int value) {
    if (!used) {
        std::cout << "[+] Вы использовали: " << name << " со значением " << value << std::endl;
        used = true;
    }
}

std::string Item::getName() const { return name; }
std::string Item::getDescription() const { return description; }
bool Item::getIsKey() const { return isKey; }
bool Item::isUsed() const { return used; }

int Item::getTotalItems() {
    return totalItems;
}