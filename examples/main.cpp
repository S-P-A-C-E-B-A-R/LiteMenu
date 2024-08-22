#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <optional>
#include <memory>
#include <cctype>
#include <cstdio>  // For sprintf
#include <string>
#include <sstream>


struct MenuItem {
    std::string title;
    bool submenu;
    bool visible;
    bool state;
    std::function<void()> action;

    MenuItem(const std::string& text,
             bool submenu,
             bool visible,
             bool state,
             std::function<void()> action = nullptr)
        : title(text),
          submenu(submenu),
          visible(visible),
          state(state),
          action(action) {}
};

struct Submenu {
    std::shared_ptr<Submenu> parentMenu;
    std::string heading;
    size_t Selection;
    bool behaveLoop;
    std::vector<MenuItem> entries;

    Submenu(std::shared_ptr<Submenu> parentMenu,
            const std::string& heading,
            bool behaveLoop)
        : parentMenu(parentMenu),
          heading(heading),
          Selection(0),
          behaveLoop(behaveLoop) {}
};

std::vector<std::shared_ptr<Submenu>> MenuTree;
std::shared_ptr<Submenu> currentMenu = nullptr;

std::shared_ptr<Submenu> findMenuByHeading(std::vector<std::shared_ptr<Submenu>>& RootTree, const std::string& heading) {
    for (auto& submenu : RootTree) {
        if (submenu->heading == heading) {
            return submenu;
        }
    }
    return nullptr;
}

void AddMenuItm(std::vector<std::shared_ptr<Submenu>>& RootTree,
                 const std::string& parentHeading,
                 const std::string& title,
                 bool submenu,
                 bool visible,
                 bool state,
                 std::function<void()> action) {
    auto parentMenu = findMenuByHeading(RootTree, parentHeading);
    if (parentMenu) {
        parentMenu->entries.emplace_back(title, submenu, visible, state, action);
    }
}

void AddMenuLvl(std::vector<std::shared_ptr<Submenu>>& RootTree,
                const std::string& parentHeading,
                const std::string& heading,
                bool loop) {
    // Find the parent menu by its heading
    auto parentMenu = findMenuByHeading(RootTree, parentHeading);

    // Create a new submenu and add it to the RootTree
    auto newSubmenu = std::make_shared<Submenu>(parentMenu, heading, loop);
    RootTree.emplace_back(newSubmenu);

    // Add the new submenu as an item to the parent menu
    if (parentMenu) {
        AddMenuItm(RootTree, parentHeading, heading, true, true, false, [newSubmenu]() {
            currentMenu = newSubmenu;
            currentMenu->Selection = 0;  // Reset the selection when entering the submenu
        });
    }

    // Add "Exit" or "Back" options
    if (RootTree.size() == 1) {
        // Root menu
        currentMenu = RootTree.back();
        // Add "Exit" to the root menu
        AddMenuItm(RootTree, heading, "Exit", false, true, false, []() {
            std::cout << "Exiting program..." << std::endl;
            exit(0);  // Or any other exit logic
        });
    } else {
        // Submenus
        // Add "Back" option to the current menu
        AddMenuItm(RootTree, heading, "Back", false, true, false, []() {
            if (currentMenu && currentMenu->parentMenu) {
                auto previousMenu = currentMenu;
                currentMenu = currentMenu->parentMenu;
                //currentMenu->Selection = std::min(previousMenu->Selection, currentMenu->entries.size() - 1); // Restore selection or set to last valid entry
                std::cout << "Going back to parent menu..." << std::endl;
            }
        });
    }
}

void DebugMenuTree(const std::vector<std::shared_ptr<Submenu>>& RootTree) {
    if (RootTree.empty()) {
        std::cout << "No Submenus in MenuTree!" << std::endl;
        return;
    }

    for (const auto& current_submenu : RootTree) {
        // Store the path termination
        std::stringstream path;
        // Start at the lowest level (current menu)
        auto current_Level = current_submenu;

        // Build the path by traversing up the menu tree
        while (current_Level != nullptr) {
            path.str("/" + current_Level->heading + path.str());
            current_Level = current_Level->parentMenu;
        }
        
        // Print Path
        std::cout << "[" << current_submenu->Selection << "]" << path.str() << "/" << std::endl;

        // Print the entries in this submenu, if any
        if (!current_submenu->entries.empty()) {
            for (const auto& entry : current_submenu->entries) {
                std::cout << "  - \"" << entry.title << "\"" << std::endl;
            }
        } else {
            std::cout << "  No entries in this submenu." << std::endl;
        }
    }
}

void printMenu() {
    if (!currentMenu) return;

    const auto& entries = currentMenu->entries;
    if (entries.empty()) return;

    std::cout << "\n" << currentMenu->heading << "\n";
    std::cout << std::string(currentMenu->heading.length(), '=') << "\n";

    for (size_t i = 0; i < entries.size(); ++i) {
        if (entries[i].visible) {
            // Determine entry type, visibility and state representation
            std::string entryType = entries[i].submenu ? "Menu" : "Item"; 
            std::string visibility = entries[i].visible ? "Vsbl" : "Invs";
            std::string state = entries[i].state ? "T" : "F";
            
            // Print each entry in the format [TypeVisibility,State] followed by the title
            std::cout << (i == currentMenu->Selection ? " > " : "   ")
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

    // Sanitize the input
    if (!std::isalpha(static_cast<unsigned char>(command))) {
        std::cout << "Invalid input!" << std::endl;
        return;
    }

    command = std::toupper(static_cast<unsigned char>(command));

    switch (command) {
        case 'W':
            if (currentMenu->Selection > 0) {
                --currentMenu->Selection;
            } else if (currentMenu->behaveLoop) {
                currentMenu->Selection = currentMenu->entries.size() - 1;
            }
            break;
        case 'S':
            if (currentMenu->Selection < currentMenu->entries.size() - 1) {
                ++currentMenu->Selection;
            } else if (currentMenu->behaveLoop) {
                currentMenu->Selection = 0;
            }
            break;
        case 'D': {
            auto& selectedEntry = currentMenu->entries[currentMenu->Selection];
            if (selectedEntry.action) {
                selectedEntry.action();
            } else if (selectedEntry.submenu) {
                currentMenu = findMenuByHeading(MenuTree, selectedEntry.title);
            }
            break;
        }
        default:
            std::cout << "Invalid input!" << std::endl;
            break;
    }
}

void toggleMenuItem() {
    if (!currentMenu) return;

    // Get the index of the currently selected item
    size_t currentIndex = currentMenu->Selection;
    if (currentIndex >= currentMenu->entries.size()) return;

    auto& item = currentMenu->entries[currentIndex];

    // Update the item state and title
    std::string newTitle = item.title;
    if (item.state) {
        newTitle = newTitle.substr(0, newTitle.find(" - On")) + " - Off";
        item.state = false;  // Toggle state to Off
    } else {
        newTitle = newTitle.substr(0, newTitle.find(" - Off")) + " - On";
        item.state = true;   // Toggle state to On
    }

    // Update the menu item
    currentMenu->entries[currentIndex].title = newTitle;
}


int main() {
    // Print the entire MenuTree structure
    DebugMenuTree(MenuTree);

    // Nest some Menus
    AddMenuLvl(MenuTree, "", "Main Menu", true);
    AddMenuLvl(MenuTree, "Main Menu", "Menu Alpha", true);
    AddMenuItm(MenuTree, "Main Menu", "Hello World", false, true, false, [](){std::cout<<"Hello World"<<std::endl;});
    AddMenuItm(MenuTree, "Main Menu", "Demo Toggle - Off", false, true, false, [](){toggleMenuItem(); });
    AddMenuLvl(MenuTree, "Menu Alpha", "Menu Alpha Lvl 1", true);
    AddMenuLvl(MenuTree, "Menu Alpha Lvl 1", "Menu Alpha Lvl 2", true);
    AddMenuLvl(MenuTree, "Main Menu", "Menu Beta", true);
    AddMenuItm(MenuTree, "Menu Beta", "Hello Planet", false, true, false, [](){std::cout<<"Hello Planet"<<std::endl;});

    // Print the entire MenuTree structure
    DebugMenuTree(MenuTree);

    // Begin Navigation Test
    std::cout << "======================" << std::endl;

    while (true){
        DebugMenuTree(MenuTree);
        printMenu();
        getInput();
    }

    return 0;
}