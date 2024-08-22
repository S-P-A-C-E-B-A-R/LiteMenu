// LiteMenu.cpp
#include "LiteMenu.h"
#include <algorithm>
#include <cctype>
#include <sstream>

LiteMenu::LiteMenu() : currentMenu(nullptr) {}

std::shared_ptr<LiteMenu::Submenu> LiteMenu::findMenuByHeading(const std::string& heading) const {
    for (const auto& submenu : menuTree) {
        if (submenu->heading == heading) {
            return submenu;
        }
    }
    return nullptr;
}

void LiteMenu::addMenuLevel(const std::string& parentHeading, const std::string& heading, bool loop) {
    auto parentMenu = findMenuByHeading(parentHeading);
    auto newSubmenu = std::make_shared<Submenu>(parentMenu, heading, loop);
    menuTree.emplace_back(newSubmenu);

    if (parentMenu) {
        addMenuItem(parentHeading, heading, true, true, false, [newSubmenu, this]() {
            currentMenu = newSubmenu;
            currentMenu->selection = 0;
        });
    }

    if (menuTree.size() == 1) {
        currentMenu = menuTree.back();
        addMenuItem(heading, "Exit", false, true, false, []() {
            std::cout << "Exiting program..." << std::endl;
            exit(0);
        });
    } else {
        addMenuItem(heading, "Back", false, true, false, [this]() {
            if (currentMenu && currentMenu->parentMenu) {
                currentMenu = currentMenu->parentMenu;
                std::cout << "Going back to parent menu..." << std::endl;
            }
        });
    }
}

void LiteMenu::addMenuItem(const std::string& parentHeading, const std::string& title, bool submenu, bool visible, bool state, std::function<void()> action) {
    auto parentMenu = findMenuByHeading(parentHeading);
    if (parentMenu) {
        parentMenu->entries.emplace_back(title, submenu, visible, state, action);
    }
}

void LiteMenu::debugMenuTree() const {
    if (menuTree.empty()) {
        std::cout << "No Submenus in MenuTree!" << std::endl;
        return;
    }

    for (const auto& currentSubmenu : menuTree) {
        std::stringstream path;
        auto currentLevel = currentSubmenu;

        while (currentLevel != nullptr) {
            path.str("/" + currentLevel->heading + path.str());
            currentLevel = currentLevel->parentMenu;
        }

        std::cout << "[" << currentSubmenu->selection << "]" << path.str() << "/" << std::endl;

        if (!currentSubmenu->entries.empty()) {
            for (const auto& entry : currentSubmenu->entries) {
                std::cout << "  - \"" << entry.title << "\"" << std::endl;
            }
        } else {
            std::cout << "  No entries in this submenu." << std::endl;
        }
    }
}

void LiteMenu::printMenu() const {
    if (!currentMenu) return;

    const auto& entries = currentMenu->entries;
    if (entries.empty()) return;

    std::cout << "\n" << currentMenu->heading << "\n";
    std::cout << std::string(currentMenu->heading.length(), '=') << "\n";

    for (size_t i = 0; i < entries.size(); ++i) {
        if (entries[i].visible) {
            std::string entryType = entries[i].submenu ? "Menu" : "Item";
            std::string visibility = entries[i].visible ? "Vsbl" : "Invs";
            std::string state = entries[i].state ? "T" : "F";
            
            std::cout << (i == currentMenu->selection ? " > " : "   ")
                      << "[" << entryType << "," << visibility << "," << state << "] "
                      << entries[i].title
                      << "\n";
        }
    }
}

void LiteMenu::getInput() {
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
            if (currentMenu->selection > 0) {
                --currentMenu->selection;
            } else if (currentMenu->behaveLoop) {
                currentMenu->selection = currentMenu->entries.size() - 1;
            }
            break;
        case 'S':
            if (currentMenu->selection < currentMenu->entries.size() - 1) {
                ++currentMenu->selection;
            } else if (currentMenu->behaveLoop) {
                currentMenu->selection = 0;
            }
            break;
        case 'D': {
            auto& selectedEntry = currentMenu->entries[currentMenu->selection];
            if (selectedEntry.action) {
                selectedEntry.action();
            } else if (selectedEntry.submenu) {
                currentMenu = findMenuByHeading(selectedEntry.title);
            }
            break;
        }
        default:
            std::cout << "Invalid input!" << std::endl;
            break;
    }
}

void LiteMenu::toggleMenuItem() {
    if (!currentMenu) return;

    size_t currentIndex = currentMenu->selection;
    if (currentIndex >= currentMenu->entries.size()) return;

    auto& item = currentMenu->entries[currentIndex];
    std::string newTitle = item.title;
    if (item.state) {
        newTitle = newTitle.substr(0, newTitle.find(" - On")) + " - Off";
        item.state = false;
    } else {
        newTitle = newTitle.substr(0, newTitle.find(" - Off")) + " - On";
        item.state = true;
    }

    currentMenu->entries[currentIndex].title = newTitle;
}
