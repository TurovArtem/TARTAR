#include "Game.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std::chrono_literals;

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

Game::Game() : gameRunning(false), ending(0), actions(0), knowsTruth(false), controlCenterLocked(true) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    initGame();
}

Game::~Game() {
    // Деструктор
}

void Game::initGame() {
    locations[0] = std::make_unique<Location>(
        "ШАТТЛ ПРИБЫТИЯ",
        "Вы на шаттле TARTAR-734. Холодный воздух пахнет озоном.\n"
        "На панели мигает: 'ТЕХНИК 734, ДОБРО ПОЖАЛОВАТЬ.'\n"
        "Система сообщает: 'СТАТУС КОМПЛЕКСА: КРИТИЧЕСКИЙ'\n"
        "В углу валяется брошенный планшет."
    );

    locations[1] = std::make_unique<Location>(
        "РЕАКТОРНЫЙ ЗАЛ",
        "Огромный зал. В центре пульсирует голубая сфера - Реактор Сознания.\n"
        "Гул оглушает. Воздух вибрирует. По стенам идут трещины.\n\n"
        "На стенах - мониторы с лицами заключённых.\n"
        "У основания реактора лежит тело техника.\n"
        "Система предупреждает: 'ГЕРМЕТИЗАЦИЯ НАРУШЕНА. УТЕЧКА ЭНЕРГИИ.'\n\n"
        "На востоке видна массивная дверь в КОНТРОЛЬНЫЙ ЦЕНТР,\n"
        "но она закрыта. Нужен КЛЮЧ ДОСТУПА."
    );

    locations[2] = std::make_unique<Location>(
        "КОНТРОЛЬНЫЙ ЦЕНТР",
        "Тишина. Комната с главной консолью.\n"
        "На экране статистика: 'АКТИВНЫХ КАМЕР: 2,347,891'\n"
        "ЭНЕРГЕТИЧЕСКИЙ ВЫХОД: 94%\n"
        "ОБСЛУЖИВАЕМЫЕ ОБЪЕКТЫ: 12\n"
        "СТАТУС ГЕРМЕТИЗАЦИИ: 42%\n\n"
        "Здесь принимаются окончательные решения."
    );

    // Создаем предметы как unique_ptr<Item>
    locations[0]->addItem(std::make_unique<Item>(
        "планшет",
        "Запись: 'Как* Имп*ерия могла соз**дать подобное?! Эта** участь хуже смерти *****\n"
        "Комплекс TARTAR-734 нестабилен. Герметизация нарушена в нескольких секторах.'",
        false
    ));

    locations[0]->addItem(std::make_unique<Item>(
        "аптечка",
        "Стандартная аптечка. Может восстановить здоровье.",
        true
    ));

    // Ключ доступа находится в реакторном зале
    locations[1]->addItem(std::make_unique<Item>(
        "ключ доступа",
        "Ключ-карта старшего техника. На обороте: 'ОНИ ЛЮДИ. МЫ - МОНСТРЫ.'\n"
        "Используйте этот ключ для открытия двери в контрольный центр.",
        true
    ));

    locations[1]->addItem(std::make_unique<Item>(
        "дневник техника",
        "Последняя запись: 'TARTAR - это тюрьма питающаяся людскими муками, они погружены в сон, длящийся столетиями, они претерпевают все муки известные человечеству.\n"
        "Империя создала самое страшное наказание для своих врагов.\n"
        "Я слышу их стоны, агонию. Я больше не могу быть в этом месте...'\n"
        "'Частичное обрушение сектора 7 нарушило герметизацию.'",
        true
    ));

    locations[2]->addItem(std::make_unique<Item>(
        "код 734",
        "Код доступа к главной консоли: 0734.\n"
        "На этикетке: 'ТОЛЬКО ДЛЯ СТАРШЕГО ТЕХНИКА. РЕШЕНИЕ ИЗМЕНИТ СУДЬБУ МИЛЛИОНОВ.'",
        true
    ));

    locations[0]->addExit("север", locations[1].get());
    locations[1]->addExit("юг", locations[0].get());
    locations[1]->addExit("восток", locations[2].get());  // Закрыт пока не будет ключа
    locations[2]->addExit("запад", locations[1].get());

    currentLocation = locations[0].get();
    gameRunning = true;
}

void Game::run() {
    showTitle();

    std::cout << "\n";
    typewriter("Год 2217. Вы - техник Империи Солнца.", 40);
    typewriter("Ваша задача: диагностика комплекса TARTAR-734.", 40);
    typewriter("Один из 12,487 объектов, где содержатся враги государства.", 40);
    typewriter("Их страх питает ваши города. Их муки - ваше процветание...", 40);
    std::cout << "\n";
    typewriter("СИСТЕМНОЕ СООБЩЕНИЕ: КОМПЛЕКС НЕСТАБИЛЕН. ЧАСТИЧНОЕ ОБРУШЕНИЕ.", 40);

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
            ending = 4;
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

        takeItem(itemName);
    }
    else if (action == "осмотреть" || action == "look" || action == "осм") {
        currentLocation->showDetailed();
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
        std::cout << "Ключевые предметы: " << player.countKeyItems() << "/3" << std::endl;
        std::cout << "Правда известна: " << (knowsTruth ? "ДА" : "НЕТ") << std::endl;
        std::cout << "Контрольный центр: " << (controlCenterLocked ? "ЗАКРЫТ" : "ОТКРЫТ") << std::endl;
        std::cout << "===============" << std::endl;
    }
    else if (action == "использовать" || action == "use") {
        std::string itemName;
        std::getline(ss, itemName);

        if (!itemName.empty() && itemName[0] == ' ') {
            itemName.erase(0, 1);
        }

        if (itemName.empty()) {
            std::cout << "Укажите предмет для использования." << std::endl;
        }
        else {
            // Использование ключа доступа для открытия контрольного центра
            if (itemName == "ключ доступа" && currentLocation->getName() == "РЕАКТОРНЫЙ ЗАЛ") {
                useKey();
            }
            // Использование других предметов
            else if (itemName == "аптечка") {
                player.useItem(itemName);
                player.heal(2);
            }
            else if (itemName == "код 734" && currentLocation->getName() == "КОНТРОЛЬНЫЙ ЦЕНТР") {
                player.useItem(itemName);
                showEndingChoice();
            }
            else {
                player.useItem(itemName);
            }
        }
    }
    else if (!cmd.empty()) {
        std::cout << "Неизвестная команда. Введите 'помощь' для списка команд." << std::endl;
    }
}

void Game::takeItem(const std::string& itemName) {
    if (currentLocation->hasItem(itemName)) {
        auto item = currentLocation->takeItem(itemName);
        if (item) {
            player.addItem(std::move(item));

            if (itemName == "ключ доступа") {
                std::cout << "\n";
                typewriter("Вы взяли ключ доступа.", 40);
                typewriter("Теперь вы можете открыть дверь в контрольный центр.", 40);
                typewriter("Используйте команду 'использовать ключ доступа' в реакторном зале.", 40);
            }
            else if (itemName == "дневник техника") {
                std::cout << "\n";
                typewriter("Вы читаете дневник...", 40);
                typewriter("Правда об Империи открывается вам.", 40);
                typewriter("TARTAR - это тюрьма питающаяся людскими муками, они погружены в сон, длящийся столетиями, они претерпевают все муки известные человечеству.", 40);
                typewriter("Империя создала самое страшное наказание для своих врагов.", 40);
                typewriter("Я слышу их стоны, агонию. Я больше не могу быть в этом месте...", 40);
                typewriter("Частичное обрушение нарушило герметизацию.", 40);
                knowsTruth = true;
                player.takeDamage(1);
            }
        }
    }
    else {
        std::cout << "Здесь нет предмета '" << itemName << "'" << std::endl;
    }
}

void Game::move(const std::string& dir) {
    Location* next = currentLocation->getExit(dir);
    if (next) {
        // Проверяем, пытается ли игрок войти в контрольный центр, когда он закрыт
        if (next->getName() == "КОНТРОЛЬНЫЙ ЦЕНТР" &&
            currentLocation->getName() == "РЕАКТОРНЫЙ ЗАЛ" &&
            controlCenterLocked) {

            std::cout << "\n";
            typewriter("ДВЕРЬ В КОНТРОЛЬНЫЙ ЦЕНТР ЗАКРЫТА.", 40);
            typewriter("ТРЕБУЕТСЯ КЛЮЧ ДОСТУПА СТАРШЕГО ТЕХНИКА.", 40);

            if (player.hasItem("ключ доступа")) {
                typewriter("У вас есть ключ. Используйте команду 'использовать ключ доступа'.", 40);
            }
            else {
                typewriter("Поищите ключ в реакторном зале.", 40);
            }

            return;
        }

        currentLocation = next;

        std::cout << "\n";
        typewriter("Вы перемещаетесь...", 40);
        pause(1);

        currentLocation->showInfo();

        // 50% шанс получить урон из-за нарушения герметизации
        if (std::rand() % 100 < 50) {
            std::cout << "\n";
            typewriter("ПРЕДУПРЕЖДЕНИЕ: ГЕРМЕТИЗАЦИЯ НАРУШЕНА!", 40);
            typewriter("Из-за частичного обрушения TARTAR-734", 40);
            typewriter("в коридорах скапливается опасная энергия.", 40);
            typewriter("Вы получаете урон при перемещении.", 40);
            player.takeDamage(1);
        }

        // Простой массив событий
        std::vector<std::string> events = {
            "Слышен грохот обрушения где-то вдалеке",
            "Мигает аварийное освещение",
            "По стене пробегают трещины"
        };

        if (std::rand() % 100 < 25) {
            std::cout << "\n[Событие] " << events[std::rand() % events.size()] << std::endl;
        }
    }
    else {
        std::cout << "Туда нельзя пройти. Доступные направления: ";
        currentLocation->showExits();
    }
}

void Game::useKey() {
    if (!player.hasItem("ключ доступа")) {
        std::cout << "У вас нет ключа доступа." << std::endl;
        return;
    }

    if (currentLocation->getName() != "РЕАКТОРНЫЙ ЗАЛ") {
        std::cout << "Здесь некуда использовать ключ." << std::endl;
        return;
    }

    std::cout << "\n";
    typewriter("Вы используете ключ доступа на двери в контрольный центр...", 40);
    pause(2);
    typewriter("СИСТЕМНОЕ СООБЩЕНИЕ: ДОСТУП РАЗРЕШЁН.", 40);
    typewriter("Дверь в контрольный центр открыта.", 40);

    controlCenterLocked = false;

    // Убираем ключ из инвентаря после использования
    player.useItem("ключ доступа");
}

void Game::showEndingChoice() {
    std::cout << "\n═══════════════════════════════════════════════════\n";
    std::cout << "           ГЛАВНАЯ КОНСОЛЬ TARTAR-734\n";
    std::cout << "═══════════════════════════════════════════════════\n";

    typewriter("ЭКРАН ГЛАВНОЙ КОНСОЛИ:", 30);
    typewriter("СТАТУС: КРИТИЧЕСКИЙ", 30);
    typewriter("ГЕРМЕТИЗАЦИЯ: 42%", 30);
    typewriter("ЭНЕРГИЯ: 94%", 30);
    typewriter("ОБСЛУЖИВАЕМЫЕ ОБЪЕКТЫ: 12 ГОРОДОВ ИМПЕРИИ", 30);
    typewriter("ЗАКЛЮЧЕННЫЕ: 2,347,891", 30);
    typewriter("ПРОИЗВОДСТВО СТРАХА: 98.7%", 30);

    std::cout << "\n═══════════════════════════════════════════════════\n";
    typewriter("ВЫ ПОДКЛЮЧИЛИ КОД 734 К ГЛАВНОЙ КОНСОЛИ.", 40);
    typewriter("СИСТЕМА ЖДЕТ ВАШЕГО РЕШЕНИЯ:", 40);

    if (knowsTruth) {
        std::cout << "\n";
        typewriter("ДНЕВНИК ТЕХНИКА РАСКРЫЛ ПРАВДУ:", 40);
        typewriter("TARTAR-734 - ЭТО ТЮРЬМА ПИТАЮЩАЯСЯ ЧЕЛОВЕЧЕСКИМИ МУКАМИ.", 40);
        typewriter("ЗАКЛЮЧЕННЫЕ ПОГРУЖЕНЫ В ВЕЧНЫЙ КОШМАРНЫЙ СОН.", 40);
        typewriter("12 ГОРОДОВ ИМПЕРИИ ЗАВИСЯТ ОТ ЭНЕРГИИ ИХ СТРАДАНИЙ.", 40);
        typewriter("ЧАСТИЧНОЕ ОБРУШЕНИЕ НАРУШИЛО ГЕРМЕТИЗАЦИЮ.", 40);
        typewriter("ЕСЛИ ОТКЛЮЧИТЬ КОМПЛЕКС, ГОРОДА ОСТАНУТСЯ БЕЗ ЗАЩИТЫ.", 40);
        typewriter("СОСЕДНИЕ ГОСУДАРСТВА УНИЧТОЖАТ БЕЗЗАЩИТНЫЕ ГОРОДА.", 40);
    }
    else {
        std::cout << "\n";
        typewriter("СИСТЕМНОЕ ПРЕДУПРЕЖДЕНИЕ:", 40);
        typewriter("ОТКЛЮЧЕНИЕ TARTAR-734 ПРИВЕДЕТ К:", 40);
        typewriter("1. ПРЕКРАЩЕНИЮ ЭНЕРГОСНАБЖЕНИЯ 12 ГОРОДОВ", 40);
        typewriter("2. ОТКЛЮЧЕНИЮ ЗАЩИТНЫХ ЩИТОВ", 40);
        typewriter("3. ВОЗМОЖНОЙ ГИБЕЛИ МИЛЛИОНОВ ГРАЖДАН", 40);
    }

    std::cout << "\n═══════════════════════════════════════════════════\n";
    std::cout << "ВЫБЕРИТЕ ПРОТОКОЛ:\n\n";

    std::cout << "[1] ПРОТОКОЛ САМОЛИКВИДАЦИИ 'РАГНАРОК'\n";
    std::cout << "    - Уничтожить комплекс TARTAR-734\n";
    std::cout << "    - 2,347,891 заключенных погибнут мгновенно\n";
    std::cout << "    - 12 городов останутся без энергии\n";
    std::cout << "    - Защитные щиты будут отключены\n\n";

    std::cout << "[2] ПРОТОКОЛ ОСВОБОЖДЕНИЯ 'ПРОМЕТЕЙ'\n";
    std::cout << "    - Освободить всех заключенных\n";
    std::cout << "    - Отключить генераторы страха\n";
    std::cout << "    - TARTAR-734 перейдет на резервное питание\n";
    std::cout << "    - Энергии хватит на 72 часа защиты городов\n\n";

    std::cout << "[3] ПРОТОКОЛ ПОДТВЕРЖДЕНИЯ 'СТАТУС-КВО'\n";
    std::cout << "    - Подтвердить текущий режим работы\n";
    std::cout << "    - TARTAR-734 продолжит функционировать\n";
    std::cout << "    - Заключенные продолжат страдать\n";
    std::cout << "    - Города будут защищены\n\n";

    std::cout << "[4] ОТЛОЖИТЬ РЕШЕНИЕ\n";
    std::cout << "    - Вернуться к исследованию\n";
    std::cout << "    - Комплекс продолжит работу\n";

    std::string choice;
    std::cout << "\nВВЕДИТЕ НОМЕР ПРОТОКОЛА [1-4]: ";
    std::getline(std::cin, choice);

    if (choice == "1") {
        ending = 1;
        gameRunning = false;
    }
    else if (choice == "2") {
        ending = 2;
        gameRunning = false;
    }
    else if (choice == "3") {
        ending = 3;
        gameRunning = false;
    }
    else if (choice == "4") {
        std::cout << "\n[Решение отложено. Консоль заблокирована на 24 часа.]\n";
    }
}

void Game::checkEnding() {
    if (!player.isAlive()) {
        std::cout << "\n";
        typewriter("Вы погибли...", 100);
        typewriter("Из-за нарушения герметизации TARTAR-734", 40);
        typewriter("опасная энергия проникла в ваш скафандр.", 40);
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
        typewriter("Из-за нарушения герметизации вы погибли.", 40);
        typewriter("Комплекс TARTAR-734 продолжает работать.", 40);
        typewriter("Страдания миллионов продолжаются.", 40);
    }
    else if (ending == 1) {
        typewriter("         КОНЦОВКА: УНИЧТОЖЕНИЕ", 40);
        std::cout << "\n";
        typewriter("Вы активировали протокол 'РАГНАРОК'.", 40);
        typewriter("TARTAR-734 уничтожен в результате цепной реакции.", 40);
        typewriter("2,347,891 заключенных погибли мгновенно.", 40);
        typewriter("Их страдания прекратились.", 40);
        typewriter("12 городов Империи остались без энергии.", 40);
        typewriter("Защитные щиты отключились через 10 секунд.", 40);
        typewriter("Соседние государства атаковали беззащитные города.", 40);
        typewriter("Миллионы граждан погибли в последовавшем конфликте.", 40);
        typewriter("Вы остановили одну катастрофу, начав другую.", 40);
    }
    else if (ending == 2) {
        typewriter("       КОНЦОВКА: ОСВОБОЖДЕНИЕ", 40);
        std::cout << "\n";
        typewriter("Вы активировали протокол 'ПРОМЕТЕЙ'.", 40);
        typewriter("Все 2,347,891 заключенных освобождены.", 40);
        typewriter("Генераторы страха отключены.", 40);
        typewriter("TARTAR-734 перешел на резервное питание.", 40);
        typewriter("Города получили 72 часа энергии для эвакуации.", 40);
        typewriter("Империя объявила вас предателем.", 40);
        typewriter("Многие города были атакованы, но люди успели эвакуироваться.", 40);
        typewriter("Вы спасли миллионы от страданий, пожертвовав империей.", 40);
    }
    else if (ending == 3) {
        typewriter("         КОНЦОВКА: ВЕРНОСТЬ", 40);
        std::cout << "\n";
        typewriter("Вы подтвердили протокол 'СТАТУС-КВО'.", 40);
        typewriter("TARTAR-734 продолжает функционировать.", 40);
        typewriter("2,347,891 человек продолжают страдать в вечном сне.", 40);
        typewriter("12 городов Империи защищены энергией комплекса.", 40);
        typewriter("Миллионы граждан живут в безопасности.", 40);
        typewriter("Империя награждает вас званием 'Герой Империи'.", 40);
        typewriter("Вы получаете повышение и поместье.", 40);
        typewriter("По ночам вам снятся крики из дневника техника...", 40);
    }
    else if (ending == 4) {
        typewriter("           КОНЦОВКА: ПРЕРЫВАНИЕ", 40);
        std::cout << "\n";
        typewriter("Вы прервали миссию.", 40);
        typewriter("Из-за нарушения герметизации комплекс нестабилен.", 40);
        typewriter("Другой техник будет отправлен для диагностики.", 40);
        typewriter("Комплекс продолжает работать...", 40);
    }

    typewriter("══════════════════════════════════════════════", 10);

    std::cout << "\n=== СТАТИСТИКА ===" << std::endl;
    std::cout << "Концовка: " << ending;
    if (ending == 1) std::cout << " (Уничтожение)";
    else if (ending == 2) std::cout << " (Освобождение)";
    else if (ending == 3) std::cout << " (Верность)";
    else if (ending == 4) std::cout << " (Прерывание)";
    else std::cout << " (Поражение)";
    std::cout << "\nЗдоровье: " << player.getHealth() << "/3" << std::endl;
    std::cout << "Собрано предметов: " << player.countKeyItems() << "/3" << std::endl;
    std::cout << "Знание правды: " << (knowsTruth ? "ДА" : "НЕТ") << std::endl;
    std::cout << "Действий выполнено: " << actions << std::endl;
    std::cout << "===================" << std::endl;

    std::cout << "\n[Нажмите Enter для выхода...]";
    std::cin.get();
}

void Game::showHelp() const {
    std::cout << "\n=== КОМАНДЫ ===" << std::endl;
    std::cout << "идти [направление] - перемещение (север, юг, восток, запад)" << std::endl;
    std::cout << "взять [предмет]     - взять предмет" << std::endl;
    std::cout << "осмотреть           - подробно осмотреть локацию" << std::endl;
    std::cout << "инвентарь           - показать инвентарь" << std::endl;
    std::cout << "использовать [предмет] - использовать предмет из инвентаря" << std::endl;
    std::cout << "помощь              - показать команды" << std::endl;
    std::cout << "цель                - показать цель" << std::endl;
    std::cout << "статус              - показать статус" << std::endl;
    std::cout << "выход               - выйти из игры" << std::endl;
}

void Game::showObjective() const {
    std::cout << "\n=== ЦЕЛЬ ===" << std::endl;
    std::cout << "1. Исследовать шаттл прибытия" << std::endl;
    std::cout << "2. Перейти в реакторный зал" << std::endl;
    std::cout << "3. Достичь контрольного центра" << std::endl;


    if (currentLocation->getName() == "РЕАКТОРНЫЙ ЗАЛ") {
        std::cout << "\n[!] Вы в реакторном зале!" << std::endl;
        if (player.hasItem("ключ доступа")) {
            std::cout << "[✓] У вас есть ключ доступа. Используйте команду 'использовать ключ доступа'" << std::endl;
        }
        else {
            std::cout << "[✗] У вас нет ключа доступа. Найдите его в этой локации." << std::endl;
        }
    }
    else if (currentLocation->getName() == "КОНТРОЛЬНЫЙ ЦЕНТР") {
        std::cout << "\n[!] Вы в контрольном центре!" << std::endl;
        if (player.hasItem("код 734")) {
            std::cout << "[✓] У вас есть код 734. Используйте команду 'использовать код 734'" << std::endl;
        }
        else {
            std::cout << "[✗] У вас нет кода 734. Найдите его в этой локации." << std::endl;
        }
    }

    std::cout << "\n[!] ВНИМАНИЕ: Нарушение герметизации!" << std::endl;
    std::cout << "   При перемещении есть 50% шанс получить урон." << std::endl;
    std::cout << "============================================" << std::endl;
}