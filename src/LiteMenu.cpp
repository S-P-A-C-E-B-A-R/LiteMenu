#include "LiteMenu.h"

// getSelection: Returns the index of the currently selected menu item.
// The method is const because it does not alter the state of the Menu object.
size_t Menu::getSelection() const {
    return currentMenu->Selection;
}

// navigate: Handles menu navigation based on the provided direction.
// - 'up' moves the selection up, looping if necessary.
// - 'down' moves the selection down, looping if necessary.
// - 'enter' triggers the action associated with the currently selected menu item.
void Menu::navigate(HMI navigation) {
    switch (navigation) {
        case HMI::enter:
            // If the selected item has an action, execute it. Otherwise, print "No Action Assigned".
            if (currentMenu->entries[currentMenu->Selection].action) {
                currentMenu->entries[currentMenu->Selection].action();
            } else {
                std::cout << "No Action Assigned" << std::endl;
            }
            break;

        case HMI::up:
            // Move up in the menu; loop to the end if at the start.
            if (currentMenu->Selection == 0) {
                currentMenu->Selection = currentMenu->entries.size() - 1;
            } else {
                --currentMenu->Selection;
            }
            break;

        case HMI::down:
            // Move down in the menu; loop to the start if at the end.
            if (currentMenu->Selection == currentMenu->entries.size() - 1) {
                currentMenu->Selection = 0;
            } else {
                ++currentMenu->Selection;
            }
            break;
    }
}

// findMenuByHeading: Searches for a submenu with the given heading.
// If found, returns a pointer to the submenu; otherwise, returns nullptr.
Menu::Submenu* Menu::findMenuByHeading(const std::string& heading) {
    for (auto& submenu : MenuTree) {
        if (submenu.getHeading() == heading) {
            return &submenu;
        }
    }
    return nullptr;
}

// AddMenuItem: Adds a menu item to the specified parent menu. 
// - Creates a new MenuItem and appends it to the parent menu's entries.
// - If the parent menu is not found, no action is taken.
void Menu::AddMenuItem(const std::string& parentHeading,
                       const std::string& title,
                       bool submenu,
                       bool visible,
                       bool state,
                       std::function<void()> action) {
    Submenu* parentMenu = findMenuByHeading(parentHeading);
    if (parentMenu) {
        parentMenu->entries.emplace_back(title, submenu, visible, state, action);
    } else {
        std::cerr << "Parent menu with heading \"" << parentHeading << "\" not found." << std::endl;
    }
}

// updateMenuItem: Updates the properties of a specific menu item.
// - Index is used to identify the item to update.
// - Optional parameters allow modification of state, title, or visibility.
void Menu::updateMenuItem(size_t index,
                          std::optional<bool> newState,
                          std::optional<std::string> newTitle,
                          std::optional<bool> newVisible) {
    if (index < currentMenu->entries.size()) {
        if (newState.has_value()) {
            currentMenu->entries[index].state = newState.value();
        }
        if (newTitle.has_value()) {
            currentMenu->entries[index].title = newTitle.value();
        }
        if (newVisible.has_value()) {
            currentMenu->entries[index].visible = newVisible.value();
        }
    } else {
        std::cerr << "Index out of bounds for updateMenuItem." << std::endl;
    }
}

// AddSubMenu: Adds a new sub-menu to the specified parent menu.
// - If the parent menu is found, a new submenu is created and added to the MenuTree vector.
// - If the parent menu is not found, no action is taken.
void Menu::AddSubMenu(const std::string& parentHeading, const std::string& heading, bool loop) {
    Submenu* parentMenu = findMenuByHeading(parentHeading);

    if (parentMenu) {
        // Add new submenu under the existing parent
        MenuTree.emplace_back(parentMenu, heading, loop);

        // Add "Back" option to the new submenu
        parentMenu->entries.emplace_back("Back", false, true, false, [this, parentMenu]() {
            currentMenu = parentMenu;
        });
    } else {
        // Create a new submenu without a parent
        MenuTree.emplace_back(nullptr, heading, loop);

        // Add "Exit" option to the new submenu
        MenuTree.back().entries.emplace_back("Exit", false, true, false, []() {
            // Handle exit action here
            std::cerr << "Exit action needs implementation" << std::endl;
        });
    }
}