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
    bool controlCenterLocked;  // Новое: статус двери в контрольный центр

    void typewriter(const std::string& text, int delay = 30) const;
    void pause(int seconds = 1) const;
    void showTitle() const;

    void initGame();
    void processCommand(const std::string& cmd);
    void showHelp() const;
    void showObjective() const;
    void takeItem(const std::string& itemName);
    void move(const std::string& dir);
    void useKey();  // Новый метод для использования ключа
    void showEndingChoice();
    void checkEnding();
    void showEnding() const;

public:
    Game();
    ~Game();
    void run();
};