#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <optional>
#include <memory>
#include <cctype>
#include "../src/LiteMenu.h"

class ExampleMenuManager {
public:
    ExampleMenuManager(const std::string& title, bool loop) 
        : rootMenu(std::make_unique<Menu>(title, loop)) {
        setup();
    }

    void run() {
        while (rootMenu) {
            printMenu();
            getInput();
        }
    }

private:
    std::unique_ptr<Menu> rootMenu;

    void setup() {
        rootMenu->AddMenuItem("Exit", false, true, false, [this]() {
                rootMenu.reset(); 
            });

        // Demo Entry
        rootMenu->AddMenuItem("Demo Action", false, true, false, [this]() {
                std::cout << "Hello World" << std::endl; 
            });

        // Demo Entry
        rootMenu->AddMenuItem("Demo Toggle - Off", false, true, false, [this]() { 
                toggleMenuItem(); 
            });

        // Demo Menu
        auto submenuDemo = new Menu("Demo SubMenu", false);
        rootMenu->AddSubMenu("Demo SubMenu", submenuDemo);
        auto subsubmenuDemo = new Menu("Demo SubSubMenu", false);
        submenuDemo->AddSubMenu("Demo SubSubMenu", subsubmenuDemo);
    }

    void printMenu() const {
        if (!rootMenu || !rootMenu->activeMenu) return;

        const auto& entries = rootMenu->activeMenu->getEntries();
        if (entries.empty()) return;

        std::cout << "\n" << rootMenu->activeMenu->getHeading() << "\n";
        std::cout << std::string(rootMenu->activeMenu->getHeading().length(), '=') << "\n";

        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].visible) {
                // Determine entry type, visibility and state representation
                std::string entrytype = entries[i].submenu ? "Menu" : "Item"; 
                std::string visibility = entries[i].visible ? "Vsbl" :"Invs";
                std::string state = entries[i].state ? "T" : "F";
                // Print each entry in the format [TypeVisibility,State] followed by the title
                std::cout << (i == rootMenu->activeMenu->getactiveSelect() ? " > " : "   ")
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
                rootMenu->activeMenu->navigate(Menu::HMI::up);
                break;
            case 'S':
                rootMenu->activeMenu->navigate(Menu::HMI::down);
                break;
            case 'D':
                rootMenu->activeMenu->navigate(Menu::HMI::enter);
                break;
            default:
                std::cout << "Invalid input!" << std::endl;
                break;
        }

        // After navigation, check if the menu has changed
        if (!rootMenu || !rootMenu->activeMenu) {
            std::cout << "Menu is no longer available!" << std::endl;
            return;
        }
    }

    void toggleMenuItem() {
        if (!rootMenu || !rootMenu->activeMenu) return;

        // Get Entry by Index
        size_t currentIndex = rootMenu->activeMenu->getactiveSelect();
        if (currentIndex >= rootMenu->activeMenu->getEntries().size()) return;

        auto& item = rootMenu->activeMenu->getEntries()[currentIndex];
        // Update Entry Values
        std::string newTitle = item.title;
        if (item.state) {
            newTitle = newTitle.substr(0, newTitle.find(" - On")) + " - Off";
            rootMenu->activeMenu->updateMenuItem(currentIndex, false, newTitle);
        } else {
            newTitle = newTitle.substr(0, newTitle.find(" - Off")) + " - On";
            rootMenu->activeMenu->updateMenuItem(currentIndex, true, newTitle);
        }
    }
};

int main() {
    ExampleMenuManager manager("Main Menu", true);
    manager.run();

    return 0;
}
