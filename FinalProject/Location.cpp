#include "Location.h"
#include "Item.h"
#include <iostream>

Location::Location(const std::string& n, const std::string& d)
    : name(n), description(d) {
}

void Location::showInfo() const {
    std::string shortName = name;
    if (name.length() > 30) {
        shortName = name.substr(0, 27) + "...";
    }

    std::cout << "\n=== " << shortName << " ===" << std::endl;
    std::cout << description << std::endl;
    showItems();
    showExits();
}

void Location::showDetailed() const {
    std::cout << "\n═══════════════════════════════════════════════════" << std::endl;
    std::cout << "  " << name << std::endl;
    std::cout << "═══════════════════════════════════════════════════" << std::endl;

    std::cout << description << std::endl;

    if (!items.empty()) {
        std::cout << "\n▐ ПРЕДМЕТЫ ДЛЯ СБОРА:" << std::endl;
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << "  • " << items[i]->getName();
            if (i < items.size() - 1) {
                std::cout << ",";
            }
            std::cout << std::endl;
        }
    }

    if (!exits.empty()) {
        std::cout << "\n▐ ДОСТУПНЫЕ НАПРАВЛЕНИЯ:" << std::endl;
        for (const auto& exit : exits) {
            std::cout << "  • [" << exit.first << "] ";
            std::cout << exit.second->getName() << std::endl;
        }
    }

    std::cout << "═══════════════════════════════════════════════════" << std::endl;
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
        std::cout << "Предметы: ";

        // Вывод предметов с запятыми между ними
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << items[i]->getName();

            // Добавляем запятую, если это не последний предмет
            if (i < items.size() - 1) {
                std::cout << ", ";
            }
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

        // Также можно добавить запятые между выходами для единообразия
        for (auto it = exits.begin(); it != exits.end(); ++it) {
            std::cout << "[" << it->first << "]";

            // Добавляем запятую, если это не последний выход
            auto next = it;
            ++next;
            if (next != exits.end()) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
}

std::string Location::getName() const { return name; }