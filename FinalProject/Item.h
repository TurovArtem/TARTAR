#pragma once
#include "GameObject.h"

// Базовый класс для предметов
class Item : public GameObject {
private:
    bool isKey;
    bool used;
    static int totalItems;  // Статический член

public:
    Item(const std::string& n, const std::string& d, bool key = false);

    // Переопределение виртуальных методов
    void inspect() const override;
    void use() override;
    void use(int value) override;

    // Перегруженные методы
    void use(const std::string& target);

    bool getIsKey() const;
    bool isUsed() const;
    bool isKeyItem() const override;

    static int getTotalItems();
};