#pragma once
#include <string>
#include <iostream>

class Item {
private:
    std::string name;
    std::string description;
    bool isKey;
    bool used;
    static int totalItems;

public:
    Item(const std::string& n, const std::string& d, bool key = false);
    virtual ~Item() = default;

    virtual void inspect() const;

    void use();
    void use(int value);

    std::string getName() const;
    std::string getDescription() const;
    bool getIsKey() const;
    bool isUsed() const;

    static int getTotalItems();
};