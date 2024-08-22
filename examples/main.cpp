// main.cpp
#include <memory>
#include "../src/LiteMenu.h"

void printMenu(const LiteMenu& menu) {
    auto currentMenuTitle = menu.getMenuTitle();
    auto currentMenuEntries = menu.getMenuEntries();
    auto currentSelection = menu.getSelection();
    
    if (currentMenuTitle.empty() || currentMenuEntries.empty()) return;

    std::cout << "\n" << currentMenuTitle << "\n";
    std::cout << std::string(currentMenuTitle.length(), '=') << "\n";

    for (size_t i = 0; i < currentMenuEntries.size(); ++i) {
        if (currentMenuEntries[i].visible) {
            std::string entryType = currentMenuEntries[i].submenu ? "Menu" : "Item";
            std::string visibility = currentMenuEntries[i].visible ? "Vsbl" : "Invs";
            std::string state = currentMenuEntries[i].state ? "T" : "F";
            
            std::cout << (i == currentSelection ? " > " : "   ")
                      << "[" << entryType << "," << visibility << "," << state << "] "
                      << currentMenuEntries[i].title
                      << "\n";
        }
    }
}

void getInput(LiteMenu& menu) {
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
            menu.navigate(LiteMenu::HMI::up);
            break;
        case 'S':
            menu.navigate(LiteMenu::HMI::down);
            break;
        case 'D':
            menu.navigate(LiteMenu::HMI::enter);
            break;
        default:
            std::cout << "Invalid input!" << std::endl;
            break;
    }

    // After navigation, check if the menu has changed
    if (menu.getMenuEntries().empty()) {
        std::cout << "Menu is no longer available!" << std::endl;
        return;
    }
}



int main() {
    LiteMenu menu;

    menu.addMenuLevel("", "Main Menu", true);

    menu.addMenuLevel("Main Menu", "Menu Alpha", true);
    menu.addMenuLevel("Menu Alpha", "Menu Alpha Lvl 1", true);
    menu.addMenuLevel("Menu Alpha Lvl 1", "Menu Alpha Lvl 2", true);

    menu.addMenuItem("Main Menu", "Hello World", false, true, false, [](){ std::cout << "Hello World" << std::endl; });
    menu.addMenuItem("Main Menu", "Demo Toggle - Off", false, true, false, [&menu]() { menu.toggleMenuItem(); });
    menu.addMenuLevel("Main Menu", "Menu Beta", true);
    menu.addMenuItem("Menu Beta", "Hello Planet", false, true, false, [](){ std::cout << "Hello Planet" << std::endl; });

    while (true) {
        //menu.debugMenuTree();
        printMenu(menu);
        getInput(menu);
    }

    return 0;
}