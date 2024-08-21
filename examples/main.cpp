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
    DemoMenuManager(const std::string& title, bool loop) 
        : DemoMenuSpawnPoint(std::make_unique<Menu>(title, loop)) {
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
        DemoMenuSpawnPoint->AddMenuItem("Exit", false, true, false, [this]() {
                DemoMenuSpawnPoint.reset(); 
            });

        // Demo Entry
        DemoMenuSpawnPoint->AddMenuItem("Demo Action", false, true, false, [this]() {
                std::cout << "Hello World" << std::endl; 
            });

        // Demo Entry
        DemoMenuSpawnPoint->AddMenuItem("Demo Toggle - Off", false, true, false, [this]() { 
                toggleMenuItem(); 
            });

        // Demo Menu
        auto submenuDemo = new Menu("Demo SubMenu", false);
        DemoMenuSpawnPoint->AddSubMenu(submenuDemo);
        auto subsubmenuDemo = new Menu("Demo Nested SubMenu", false);
        submenuDemo->AddSubMenu(subsubmenuDemo);
    }

    void printMenu() const {
        if (!DemoMenuSpawnPoint || !DemoMenuSpawnPoint->activeMenu) return;

        const auto& entries = DemoMenuSpawnPoint->activeMenu->getEntries();
        if (entries.empty()) return;

        std::cout << "\n" << DemoMenuSpawnPoint->activeMenu->getHeading() << "\n";
        std::cout << std::string(DemoMenuSpawnPoint->activeMenu->getHeading().length(), '=') << "\n";

        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].visible) {
                // Determine entry type, visibility and state representation
                std::string entrytype = entries[i].submenu ? "Menu" : "Item"; 
                std::string visibility = entries[i].visible ? "Vsbl" :"Invs";
                std::string state = entries[i].state ? "T" : "F";
                // Print each entry in the format [TypeVisibility,State] followed by the title
                std::cout << (i == DemoMenuSpawnPoint->activeMenu->getactiveSelect() ? " > " : "   ")
                        << "[" << entrytype << "," << visibility << "," << state << "] "
                        << entries[i].title
                        << "\n";
            }
        }
    }

    void getInput() {
        char command;
        std::cout << "\nCommand (w, s, d): ";
        std::cin >> command;

        // Sanitize the input
        if (!std::isalpha(static_cast<unsigned char>(command))) {
            std::cout << "Invalid input!" << std::endl;
            return;
        }

        command = std::toupper(static_cast<unsigned char>(command));

        switch (command) {
            case 'W':
                DemoMenuSpawnPoint->activeMenu->navigate(Menu::HMI::up);
                break;
            case 'S':
                DemoMenuSpawnPoint->activeMenu->navigate(Menu::HMI::down);
                break;
            case 'D':
                DemoMenuSpawnPoint->activeMenu->navigate(Menu::HMI::enter);
                break;
            default:
                std::cout << "Invalid input!" << std::endl;
                break;
        }

        // After navigation, check if the menu has changed
        if (!DemoMenuSpawnPoint || !DemoMenuSpawnPoint->activeMenu) {
            std::cout << "Menu is no longer available!" << std::endl;
            return;
        }
    }

    void toggleMenuItem() {
        if (!DemoMenuSpawnPoint || !DemoMenuSpawnPoint->activeMenu) return;

        // Get Entry by Index
        size_t currentIndex = DemoMenuSpawnPoint->activeMenu->getactiveSelect();
        if (currentIndex >= DemoMenuSpawnPoint->activeMenu->getEntries().size()) return;

        auto& item = DemoMenuSpawnPoint->activeMenu->getEntries()[currentIndex];
        // Update Entry Values
        std::string newTitle = item.title;
        if (item.state) {
            newTitle = newTitle.substr(0, newTitle.find(" - On")) + " - Off";
            DemoMenuSpawnPoint->activeMenu->updateMenuItem(currentIndex, false, newTitle);
        } else {
            newTitle = newTitle.substr(0, newTitle.find(" - Off")) + " - On";
            DemoMenuSpawnPoint->activeMenu->updateMenuItem(currentIndex, true, newTitle);
        }
    }
};

int main() {
    DemoMenuManager manager("Main Menu", true);
    manager.run();

    return 0;
}
