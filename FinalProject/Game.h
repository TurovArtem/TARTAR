#pragma once
#include "Player.h"
#include "Location.h"
#include <array>
#include <memory>
#include <thread>
#include <chrono>

class Game {
private:
    Player player;
    std::array<std::unique_ptr<Location>, 3> locations;
    Location* currentLocation;
    bool gameRunning;
    int ending;
    int actions;
    bool knowsTruth;

    // Методы для анимации (добавлен const)
    void typewriter(const std::string& text, int delay = 30) const;
    void pause(int seconds = 1) const;
    void showTitle() const;

    // Основные методы
    void initGame();
    void processCommand(const std::string& cmd);
    void showHelp() const;
    void showObjective() const;
    void move(const std::string& dir);
    void checkEnding();
    void showEnding() const;

public:
    Game();
    void run();
};