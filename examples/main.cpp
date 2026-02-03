#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <optional>
#include <memory>
#include <cctype>
#include "../src/LiteMenu.h"

class MenuApplication {
public:
    MenuApplication(const std::string& title, bool loop) 
        : mainMenu(std::make_unique<Menu>(title, loop)) {
        initializeMenus();
    }

    void run() {
        while (mainMenu) {
            displayCurrentMenu();
            processUserInput();
        }
    }

private:
    std::unique_ptr<Menu> mainMenu;

    void initializeMenus() {
        mainMenu->AddMenuItem("Exit Application", false, true, false, [this]() {
            mainMenu.reset(); 
        });

        // Level 0 Actions
        mainMenu->AddMenuItem("Level 0 Action", false, true, false, [this]() {
            std::cout << "Level 0 action executed!" << std::endl; 
        });

        mainMenu->AddMenuItem("Level 0 Toggle - Off", false, true, false, [this]() { 
            toggleCurrentMenuItem(); 
        });

        // Level 1 Menu
        auto level1Menu = new Menu("Level 1 Menu", false);
        mainMenu->AddSubMenu(level1Menu);
        
        // Level 1 Actions
        level1Menu->AddMenuItem("Level 1 Action", false, true, false, [this]() {
            std::cout << "Level 1 action executed!" << std::endl;
        });
        
        // Level 2 Menu
        auto level2Menu = new Menu("Level 2 Menu", false);
        level1Menu->AddSubMenu(level2Menu);
        
        // Level 2 Menu Items
        level2Menu->AddMenuItem("Level 2 Action 1", false, true, false, [this]() {
            std::cout << "Level 2 action 1 executed!" << std::endl; 
        });
        level2Menu->AddMenuItem("Level 2 Action 2", false, true, false, [this]() {
            std::cout << "Level 2 action 2 executed!" << std::endl; 
        });
        level2Menu->AddMenuItem("Level 2 Toggle - Off", false, true, false, [this]() {
            toggleCurrentMenuItem();  
        });
    }

    void displayCurrentMenu() const {
        if (!mainMenu || !mainMenu->activeMenu) return;

        const auto& entries = mainMenu->activeMenu->getEntries();
        if (entries.empty()) return;

        std::cout << "\n" << mainMenu->activeMenu->getHeading() << "\n";
        std::cout << std::string(mainMenu->activeMenu->getHeading().length(), '=') << "\n";

        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].visible) {
                // Determine entry type, visibility and state representation
                std::string entrytype = entries[i].submenu ? "Menu" : "Item"; 
                std::string visibility = entries[i].visible ? "Vsbl" :"Invs";
                std::string state = entries[i].state ? "T" : "F";
                // Print each entry in the format [TypeVisibility,State] followed by the title
                std::cout << (i == mainMenu->activeMenu->getactiveSelect() ? " > " : "   ")
                        << "[" << entrytype << "," << visibility << "," << state << "] "
                        << entries[i].title
                        << "\n";
            }
        }
    }

    void processUserInput() {
        char command;
        std::cout << "\nNavigation (w=up, s=down, d=select): ";
        std::cin >> command;

        // Sanitize the input
        if (!std::isalpha(static_cast<unsigned char>(command))) {
            std::cout << "Invalid input!" << std::endl;
            return;
        }

        command = std::toupper(static_cast<unsigned char>(command));

        switch (command) {
            case 'W':
                mainMenu->navigate(Menu::HMI::up);
                break;
            case 'S':
                mainMenu->navigate(Menu::HMI::down);
                break;
            case 'D':
                mainMenu->navigate(Menu::HMI::enter);
                break;
            default:
                std::cout << "Invalid input!" << std::endl;
                break;
        }

        // After navigation, check if the menu is still available
        if (!mainMenu || !mainMenu->activeMenu) {
            std::cout << "Application is no longer available!" << std::endl;
            return;
        }
    }

    void toggleCurrentMenuItem() {
        if (!mainMenu || !mainMenu->activeMenu) return;

        // Get Entry by Index
        size_t currentIndex = mainMenu->activeMenu->getactiveSelect();
        if (currentIndex >= mainMenu->activeMenu->getEntries().size()) return;

        auto& item = mainMenu->activeMenu->getEntries()[currentIndex];
        // Update Entry Values
        std::string newTitle = item.title;
        if (item.state) {
            newTitle = newTitle.substr(0, newTitle.find(" - On")) + " - Off";
            mainMenu->activeMenu->updateMenuItem(currentIndex, false, newTitle);
        } else {
            newTitle = newTitle.substr(0, newTitle.find(" - Off")) + " - On";
            mainMenu->activeMenu->updateMenuItem(currentIndex, true, newTitle);
        }
    }
};

int main() {
    MenuApplication application("Main Application", true);
    application.run();
    
    return 0;
}
