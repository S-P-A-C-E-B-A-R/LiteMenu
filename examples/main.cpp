// main.cpp
#include "../src/LiteMenu.h"

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

    menu.debugMenuTree();

    while (true) {
        menu.debugMenuTree();
        menu.printMenu();
        menu.getInput();
    }

    return 0;
}