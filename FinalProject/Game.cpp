#include "Game.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std::chrono_literals;

// ==================== МЕТОДЫ АНИМАЦИИ (с const) ====================

void Game::typewriter(const std::string& text, int delay) const {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

void Game::pause(int seconds) const {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void Game::showTitle() const {
#ifdef _WIN32
    system("cls");
#endif

    std::cout << "\n\n";
    typewriter("████████╗ █████╗ ██████╗ ████████╗ █████╗ ██████╗ ", 10);
    typewriter("╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗██╔══██╗", 10);
    typewriter("   ██║   ███████║██████╔╝   ██║   ███████║██████╔╝", 10);
    typewriter("   ██║   ██╔══██║██╔══██╗   ██║   ██╔══██║██╔══██╗", 10);
    typewriter("   ██║   ██║  ██║██║  ██║   ██║   ██║  ██║██║  ██║", 10);
    typewriter("   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝", 10);

    std::cout << "\n";
    typewriter("              ТЕКСТОВЫЙ КВЕСТ", 40);
    std::cout << "\n";
    typewriter("══════════════════════════════════════════════", 10);

    pause(2);
}

Game::Game() : gameRunning(false), ending(0), actions(0), knowsTruth(false) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    initGame();
}

void Game::initGame() {
    locations[0] = std::make_unique<Location>(
        "ШАТТЛ ПРИБЫТИЯ",
        "Вы на шаттле TARTAR-734. Холодный воздух пахнет озоном.\n"
        "На панели мигает: 'ТЕХНИК 734, ДОБРО ПОЖАЛОВАТЬ.'\n\n"
        "В углу валяется брошенный планшет."
    );

    locations[1] = std::make_unique<Location>(
        "РЕАКТОРНЫЙ ЗАЛ",
        "Огромный зал. В центре пульсирует голубая сфера - Реактор Сознания.\n"
        "Гул оглушает. Воздух вибрирует.\n\n"
        "На стенах - мониторы с лицами заключённых.\n"
        "У основания реактора лежит тело техника."
    );

    locations[2] = std::make_unique<Location>(
        "КОНТРОЛЬНЫЙ ЦЕНТР",
        "Тишина. Комната с главной консолью.\n"
        "На экране статистика: 'АКТИВНЫХ КАМЕР: 2,347,891'\n\n"
        "Здесь принимаются окончательные решения."
    );

    locations[0]->addItem(std::make_unique<Item>(
        "планшет",
        "Запись: 'Экипаж шаттла 12 отказался выходить. Причина: голоса в стенах.'",
        false
    ));

    locations[0]->addItem(std::make_unique<Item>(
        "аптечка",
        "Стандартная аптечка. Может восстановить здоровье.",
        true
    ));

    locations[1]->addItem(std::make_unique<Item>(
        "ключ доступа",
        "Ключ-карта старшего техника. На обороте: 'ОНИ ЛЮДИ. МЫ - МОНСТРЫ.'",
        true
    ));

    locations[1]->addItem(std::make_unique<Item>(
        "дневник техника",
        "Последняя запись: 'TARTAR питается страхом заключённых.\n"
        "Империя построена на их муках. Я не могу продолжать...'",
        true
    ));

    locations[2]->addItem(std::make_unique<Item>(
        "код 734",
        "Код доступа к главной консоли: 0734.",
        true
    ));

    locations[0]->addExit("север", locations[1].get());
    locations[1]->addExit("юг", locations[0].get());
    locations[1]->addExit("восток", locations[2].get());
    locations[2]->addExit("запад", locations[1].get());

    currentLocation = locations[0].get();
    gameRunning = true;
}

void Game::showHelp() const {
    std::cout << "\n=== КОМАНДЫ ===" << std::endl;
    std::cout << "идти [направление] - перемещение (север, юг, восток, запад)" << std::endl;
    std::cout << "взять [предмет]     - взять предмет" << std::endl;
    std::cout << "инвентарь          - показать инвентарь" << std::endl;
    std::cout << "использовать       - взаимодействовать" << std::endl;
    std::cout << "помощь             - показать команды" << std::endl;
    std::cout << "цель               - показать цель" << std::endl;
    std::cout << "статус             - показать статус" << std::endl;
    std::cout << "выход              - выйти из игры" << std::endl;
}

void Game::showObjective() const {
    std::cout << "\n=== ЦЕЛЬ ===" << std::endl;
    std::cout << "1. Исследовать комплекс TARTAR-734" << std::endl;
    std::cout << "2. Достичь контрольного центра" << std::endl;
    std::cout << "3. Принять решение о судьбе комплекса" << std::endl;
    std::cout << "=================" << std::endl;
}

void Game::run() {
    showTitle();

    std::cout << "\n";
    typewriter("Год 2217. Вы - техник Империи Солнца.", 40);
    typewriter("Ваша задача: диагностика комплекса TARTAR-734.", 40);
    typewriter("Один из 12,487 объектов, где содержатся враги государства.", 40);
    typewriter("Их страх питает ваши города. Их муки - ваше процветание...", 40);

    pause(2);

    showObjective();

    std::cout << "\n[Нажмите Enter, чтобы начать...]";
    std::cin.ignore();

    currentLocation->showInfo();

    std::string command;
    while (gameRunning && player.isAlive()) {
        if (actions > 0 && actions % 4 == 0) {
            showObjective();
        }

        std::cout << "\n> ";
        std::getline(std::cin, command);

        if (command == "выход") {
            std::cout << "\n";
            typewriter("Вы выходите из игры...", 40);
            ending = 2;
            gameRunning = false;
            break;
        }

        actions++;
        processCommand(command);
        checkEnding();
    }

    showEnding();
}

void Game::processCommand(const std::string& cmd) {
    std::stringstream ss(cmd);
    std::string action;
    ss >> action;

    std::transform(action.begin(), action.end(), action.begin(), ::tolower);

    if (action == "идти" || action == "go") {
        std::string dir;
        ss >> dir;
        move(dir);
    }
    else if (action == "взять" || action == "take") {
        std::string itemName;
        std::getline(ss, itemName);

        if (!itemName.empty() && itemName[0] == ' ') {
            itemName.erase(0, 1);
        }

        if (itemName.empty()) {
            std::cout << "Укажите название предмета." << std::endl;
            return;
        }

        if (currentLocation->hasItem(itemName)) {
            auto item = currentLocation->takeItem(itemName);
            if (item) {
                player.addItem(std::move(item));

                if (itemName == "дневник техника") {
                    std::cout << "\n";
                    typewriter("Вы читаете дневник...", 40);
                    typewriter("Правда об Империи открывается вам.", 40);
                    typewriter("TARTAR питается человеческими страданиями.", 40);
                    knowsTruth = true;
                    player.takeDamage(1);
                }
            }
        }
        else {
            std::cout << "Здесь нет предмета '" << itemName << "'" << std::endl;
        }
    }
    else if (action == "инвентарь" || action == "inventory" || action == "инв") {
        player.showInventory();
    }
    else if (action == "помощь" || action == "help") {
        showHelp();
    }
    else if (action == "цель" || action == "objective") {
        showObjective();
    }
    else if (action == "статус" || action == "status") {
        std::cout << "\n=== СТАТУС ===" << std::endl;
        std::cout << "Здоровье: " << player.getHealth() << "/3" << std::endl;
        std::cout << "Предметы: " << player.countKeyItems() << "/3" << std::endl;
        std::cout << "Правда известна: " << (knowsTruth ? "ДА" : "НЕТ") << std::endl;
        std::cout << "===============" << std::endl;
    }
    else if (action == "использовать" || action == "use") {
        if (currentLocation->getName() == "ШАТТЛ ПРИБЫТИЯ") {
            std::cout << "\nОсмотреть иллюминатор? (да/нет): ";
            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "да" || choice == "д") {
                std::cout << "\n";
                typewriter("На стекле следы когтей...", 40);
                typewriter("И слово, нацарапанное кровью:", 40);
                typewriter("ПОМОГИ", 100);
                player.takeDamage(1);
            }
        }
        else if (currentLocation->getName() == "РЕАКТОРНЫЙ ЗАЛ") {
            std::cout << "\nОсмотреть тело техника? (да/нет): ";
            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "да" || choice == "д") {
                std::cout << "\n";
                typewriter("В руке техника записка:", 40);
                typewriter("'Они всё ещё чувствуют. Они всё ещё страдают.'", 40);
            }
        }
        else if (currentLocation->getName() == "КОНТРОЛЬНЫЙ ЦЕНТР") {
            if (player.hasItem("код 734")) {
                std::cout << "\n=== ГЛАВНАЯ КОНСОЛЬ ===" << std::endl;
                std::cout << "Введите код доступа: ";

                std::string input;
                std::getline(std::cin, input);

                if (input == "0734") {
                    std::cout << "\n";
                    typewriter("Доступ разрешён.", 40);

                    if (knowsTruth) {
                        typewriter("Вы знаете правду о TARTAR.", 40);
                        typewriter("2,347,891 человек мучаются прямо сейчас.", 40);

                        std::cout << "\nВаш выбор:\n";
                        std::cout << "1. УНИЧТОЖИТЬ комплекс (остановить страдания)\n";
                        std::cout << "2. ОСВОБОДИТЬ заключённых (предать Империю)\n";
                        std::cout << "3. ПРОДОЛЖИТЬ работу (остаться верным)\n";
                    }
                    else {
                        typewriter("Система TARTAR работает стабильно.", 40);

                        std::cout << "\nВаш выбор:\n";
                        std::cout << "1. Активировать аварийное отключение\n";
                        std::cout << "2. Запустить процедуру освобождения\n";
                        std::cout << "3. Подтвердить текущий режим\n";
                    }

                    std::cout << "\nВыбор (1-3): ";
                    std::string choice;
                    std::getline(std::cin, choice);

                    if (choice == "1") ending = 1;
                    else if (choice == "2") ending = 2;
                    else if (choice == "3") ending = 3;
                    else {
                        std::cout << "Неверный выбор. Принято решение 3." << std::endl;
                        ending = 3;
                    }

                    gameRunning = false;
                }
                else {
                    std::cout << "Неверный код!" << std::endl;
                    player.takeDamage(1);
                }
            }
            else {
                std::cout << "Нужен код доступа!" << std::endl;
            }
        }
    }
    else if (!cmd.empty()) {
        std::cout << "Неизвестная команда. Введите 'помощь' для списка команд." << std::endl;
    }
}

void Game::move(const std::string& dir) {
    Location* next = currentLocation->getExit(dir);
    if (next) {
        currentLocation = next;

        std::cout << "\n";
        typewriter("Вы перемещаетесь...", 40);
        pause(1);

        currentLocation->showInfo();

        player.takeDamage(1);

        if (std::rand() % 100 < 25) {
            std::cout << "\n";
            typewriter("Вы слышите странные звуки...", 40);
        }
    }
    else {
        std::cout << "Туда нельзя пройти. Доступные направления: ";
        currentLocation->showExits();
    }
}

void Game::checkEnding() {
    if (!player.isAlive()) {
        std::cout << "\n";
        typewriter("Вы погибли...", 100);
        typewriter("Империя найдёт нового техника.", 40);
        typewriter("Машина продолжит работать.", 40);
        ending = 0;
        gameRunning = false;
    }
}

void Game::showEnding() const {
    std::cout << "\n\n";
    typewriter("══════════════════════════════════════════════", 10);

    if (ending == 0) {
        typewriter("           КОНЦОВКА: ПОРАЖЕНИЕ", 40);
        std::cout << "\n";
        typewriter("Вы не выполнили свою миссию.", 40);
        typewriter("Комплекс TARTAR-734 продолжает работать.", 40);
        typewriter("Страдания миллионов продолжаются.", 40);
    }
    else if (ending == 1) {
        typewriter("         КОНЦОВКА: УНИЧТОЖЕНИЕ", 40);
        std::cout << "\n";
        typewriter("Вы активировали протокол самоликвидации.", 40);
        typewriter("TARTAR-734 уничтожен.", 40);
        typewriter("2,347,891 человек мгновенно погибли.", 40);
        typewriter("Их муки прекратились навсегда.", 40);
        typewriter("Империя в ярости, но правда похоронена.", 40);
    }
    else if (ending == 2) {
        typewriter("       КОНЦОВКА: ОСВОБОЖДЕНИЕ", 40);
        std::cout << "\n";
        typewriter("Вы освободили всех заключённых.", 40);
        typewriter("2,347,891 человек пробудились от кошмара.", 40);
        typewriter("Империя потеряла контроль над комплексом.", 40);
        typewriter("Вы - предатель в глазах Империи.", 40);
        typewriter("Но для миллионов - вы герой.", 40);
    }
    else if (ending == 3) {
        typewriter("         КОНЦОВКА: ВЕРНОСТЬ", 40);
        std::cout << "\n";
        typewriter("Вы подтвердили текущий режим работы.", 40);
        typewriter("TARTAR-734 продолжает функционировать.", 40);
        typewriter("2,347,891 человек продолжают страдать.", 40);
        typewriter("Империя награждает вас за службу.", 40);
        typewriter("Вы получаете повышение и награды.", 40);
    }

    typewriter("══════════════════════════════════════════════", 10);

    std::cout << "\n=== СТАТИСТИКА ===" << std::endl;
    std::cout << "Концовка: " << ending;
    if (ending == 1) std::cout << " (Уничтожение)";
    else if (ending == 2) std::cout << " (Освобождение)";
    else if (ending == 3) std::cout << " (Верность)";
    else std::cout << " (Поражение)";
    std::cout << "\nЗдоровье: " << player.getHealth() << "/3" << std::endl;
    std::cout << "Собрано предметов: " << player.countKeyItems() << "/3" << std::endl;
    std::cout << "Знание правды: " << (knowsTruth ? "ДА" : "НЕТ") << std::endl;
    std::cout << "Действий выполнено: " << actions << std::endl;
    std::cout << "===================" << std::endl;

    std::cout << "\n[Нажмите Enter для выхода...]";
    std::cin.get();
}