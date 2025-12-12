#include "GameObject.h"

GameObject::GameObject(const std::string& n, const std::string& d)
    : name(n), description(d) {
}

void GameObject::inspect() const {
    std::cout << name << ": " << description << std::endl;
}

void GameObject::use() {
    std::cout << "Вы пытаетесь использовать " << name << std::endl;
}

void GameObject::use(int value) {
    std::cout << "Вы пытаетесь использовать " << name
        << " со значением " << value << std::endl;
}

std::string GameObject::getName() const { return name; }
std::string GameObject::getDescription() const { return description; }

bool GameObject::isKeyItem() const {
    return false; // По умолчанию не ключевой предмет
}