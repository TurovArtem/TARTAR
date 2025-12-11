#pragma once
#include <string>

class Item {
private:
    std::string name;
    std::string description;
    bool isKey;

public:
    Item(const std::string& n, const std::string& d, bool key = false);

    std::string getName() const;
    std::string getDescription() const;
    bool getIsKey() const;
    void inspect() const;
};