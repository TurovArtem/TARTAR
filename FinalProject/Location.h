#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Item.h"

class Location {
private:
    std::string name;
    std::string description;
    std::vector<std::unique_ptr<Item>> items;
    std::map<std::string, Location*> exits;

public:
    Location(const std::string& n, const std::string& d);

    void showInfo() const;

    void addItem(std::unique_ptr<Item> item);
    std::unique_ptr<Item> takeItem(const std::string& name);
    bool hasItem(const std::string& name) const;
    void showItems() const;

    void addExit(const std::string& dir, Location* loc);
    Location* getExit(const std::string& dir) const;
    void showExits() const;

    std::string getName() const;
};