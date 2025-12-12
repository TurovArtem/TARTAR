#pragma once
#include "Item.h"

// Производный класс для расходуемых предметов
class Consumable : public Item {
private:
    int healingValue;

public:
    Consumable(const std::string& n, const std::string& d, int heal);

    // Переопределение методов
    void inspect() const override;
    void use() override;

    int getHealingValue() const;
    void consume();
};