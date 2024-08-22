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

int main() {
    LiteMenu menu;

    menu.addMenuLevel("", "Main Menu", true);
    menu.addMenuLevel("Main Menu", "Menu Alpha", true);
    menu.addMenuItem("Main Menu", "Hello World", false, true, false, [](){ std::cout << "Hello World" << std::endl; });
    menu.addMenuItem("Main Menu", "Demo Toggle - Off", false, true, false, [&menu]() { menu.toggleMenuItem(); });
    menu.addMenuLevel("Menu Alpha", "Menu Alpha Lvl 1", true);
    menu.addMenuLevel("Menu Alpha Lvl 1", "Menu Alpha Lvl 2", true);
    menu.addMenuLevel("Main Menu", "Menu Beta", true);
    menu.addMenuItem("Menu Beta", "Hello Planet", false, true, false, [](){ std::cout << "Hello Planet" << std::endl; });

    //menu.debugMenuTree();

    while (true) {
        //menu.debugMenuTree();
        printMenu(menu);
        menu.getInput();
        //getInput(menu);
    }

    return 0;
}