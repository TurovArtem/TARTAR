#include "Item.h"
#include <iostream>

Item::Item(const std::string& n, const std::string& d, bool key)
    : name(n), description(d), isKey(key) {
}

std::string Item::getName() const { return name; }
std::string Item::getDescription() const { return description; }
bool Item::getIsKey() const { return isKey; }

void Item::inspect() const {
    std::cout << name << ": " << description;
    if (isKey) std::cout << " (ключевой)";
    std::cout << std::endl;
}