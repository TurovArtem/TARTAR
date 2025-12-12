#include "Consumable.h"
#include <iostream>

Consumable::Consumable(const std::string& n, const std::string& d, int heal)
    : Item(n, d, true), healingValue(heal) {
}

void Consumable::inspect() const {
    std::cout << "Расходуемый предмет: " << getName() << std::endl;
    std::cout << getDescription() << std::endl;
    std::cout << "Восстанавливает: " << healingValue << " здоровья" << std::endl;
}

void Consumable::use() {
    std::cout << "[+] Вы используете: " << getName()
        << " (восстановление: " << healingValue << ")" << std::endl;
    consume();
}

int Consumable::getHealingValue() const { return healingValue; }

void Consumable::consume() {
    std::cout << "Вы чувствуете прилив сил." << std::endl;
}