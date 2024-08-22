#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <optional>
#include <memory>
#include <cctype>
#include "../src/LiteMenu.h"

class DemoMenuManager {
public:
    DemoMenuManager() {
        setup();
    }

    void run() {
        while (DemoMenuSpawnPoint) {
            printMenu();
            getInput();
        }
    }

private:
    std::unique_ptr<Menu> DemoMenuSpawnPoint;

    void setup() {
        // Demo Menu Root
        DemoMenuSpawnPoint->AddSubMenu("Main Menu", "Main Menu", false);

        // Demo Entry
        DemoMenuSpawnPoint->AddMenuItem("Main Menu", "Demo Action", false, true, false, []() { 
            std::cout << "Hello World" << std::endl; 
        });

        // Demo Toggle Entry
        DemoMenuSpawnPoint->AddMenuItem("Main Menu", "Demo Toggle - Off", false, true, false, [this]() { 
            toggleMenuItem(); 
        });

        // Demo SubMenu
        DemoMenuSpawnPoint->AddSubMenu("Main Menu", "Demo SubMenu", false);
    }

    void printMenu() const {
        if (!DemoMenuSpawnPoint) return;

        const auto& entries = DemoMenuSpawnPoint->getEntries();
        if (entries.empty()) return;

        std::cout << "\n" << DemoMenuSpawnPoint->getHeading() << "\n";
        std::cout << std::string(DemoMenuSpawnPoint->getHeading().length(), '=') << "\n";

        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].visible) {
                std::string entryType = entries[i].submenu ? "Menu" : "Item"; 
                std::string visibility = entries[i].visible ? "Vsbl" : "Invs";
                std::string state = entries[i].state ? "T" : "F";
                std::cout << (i == DemoMenuSpawnPoint->getSelection() ? " > " : "   ")
                        << "[" << entryType << "," << visibility << "," << state << "] "
                        << entries[i].title
                        << "\n";
            }
        }
    }

    void getInput() {
        char command;
        std::cout << "\nCommand (w, s, d): ";
        std::cin >> command;

        if (!std::isalpha(static_cast<unsigned char>(command))) {
            std::cout << "Invalid input!" << std::endl;
            return;
        }

        command = std::toupper(static_cast<unsigned char>(command));

        switch (command) {
            case 'W':
                DemoMenuSpawnPoint->navigate(Menu::HMI::up);
                break;
            case 'S':
                DemoMenuSpawnPoint->navigate(Menu::HMI::down);
                break;
            case 'D':
                DemoMenuSpawnPoint->navigate(Menu::HMI::enter);
                break;
            default:
                std::cout << "Invalid input!" << std::endl;
                break;
        }
    }

    void toggleMenuItem() {
        if (!DemoMenuSpawnPoint) return;

        size_t currentIndex = DemoMenuSpawnPoint->getSelection();
        if (currentIndex >= DemoMenuSpawnPoint->getEntries().size()) return;

        auto& item = DemoMenuSpawnPoint->getEntries()[currentIndex];
        std::string newTitle = item.title;
        if (item.state) {
            newTitle = newTitle.substr(0, newTitle.find(" - On")) + " - Off";
            DemoMenuSpawnPoint->updateMenuItem(currentIndex, false, newTitle);
        } else {
            newTitle = newTitle.substr(0, newTitle.find(" - Off")) + " - On";
            DemoMenuSpawnPoint->updateMenuItem(currentIndex, true, newTitle);
        }
    }
};

int main() {
    DemoMenuManager manager;
    manager.run();

    return 0;
}
