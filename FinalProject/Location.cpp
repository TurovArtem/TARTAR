#include "Location.h"
#include <iostream>

Location::Location(const std::string& n, const std::string& d)
    : name(n), description(d) {
}

void Location::showInfo() const {
    std::cout << "\n=== " << name << " ===" << std::endl;
    std::cout << description << std::endl;
    showItems();
    showExits();
}

void Location::addItem(std::unique_ptr<Item> item) {
    items.push_back(std::move(item));
}

std::unique_ptr<Item> Location::takeItem(const std::string& name) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if ((*it)->getName() == name) {
            std::unique_ptr<Item> item = std::move(*it);
            items.erase(it);
            return item;
        }
    }
    return nullptr;
}

bool Location::hasItem(const std::string& name) const {
    for (const auto& item : items) {
        if (item->getName() == name) return true;
    }
    return false;
}

void Location::showItems() const {
    if (!items.empty()) {
        std::cout << "Здесь есть: ";
        for (const auto& item : items) {
            std::cout << item->getName() << " ";
        }
        std::cout << std::endl;
    }
}

void Location::addExit(const std::string& dir, Location* loc) {
    exits[dir] = loc;
}

Location* Location::getExit(const std::string& dir) const {
    auto it = exits.find(dir);
    return (it != exits.end()) ? it->second : nullptr;
}

void Location::showExits() const {
    if (!exits.empty()) {
        std::cout << "Выходы: ";
        for (const auto& exit : exits) {
            std::cout << "[" << exit.first << "] ";
        }
        std::cout << std::endl;
    }
}

std::string Location::getName() const { return name; }