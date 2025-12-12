#pragma once
#include <string>
#include <iostream>

// Базовый класс для всех игровых объектов
class GameObject {
protected:
    std::string name;
    std::string description;

public:
    GameObject(const std::string& n, const std::string& d);
    virtual ~GameObject() = default;

    // Виртуальные методы для полиморфизма
    virtual void inspect() const;
    virtual void use();
    virtual void use(int value);

    std::string getName() const;
    std::string getDescription() const;

    // Виртуальный метод для проверки типа предмета
    virtual bool isKeyItem() const;
};