#include "LiteMenu.h"

// Constructor: Initializes the Menu object with a heading and looping behavior.
// The active menu and selection are set to the current menu and the first item respectively.
Menu::Menu(const std::string& heading, bool loop) {
    // Create the root submenu (implicit population of fields!)
    MenuTree.emplace_back(nullptr, heading, loop); // nullptr indicates it's the root menu

    // Set the current menu to the root menu
    this->currentMenu = &MenuTree.back();   ///< Pointer to the currently active menu (for navigation).
}

// Destructor: Cleans up any sub-menus created by this menu.
Menu::~Menu() {
    // No need to manually delete anything because MenuTree contains objects, not pointers.
    // The vector will automatically clean up the Submenu objects when it is destroyed.
}

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
            // Move selection up, if possible. If at the top and looping is enabled, wrap to the bottom.
            if (currentMenu->Selection > 0) {
                --currentMenu->Selection;
            } else if (currentMenu->behaveLoop) {
                currentMenu->Selection = currentMenu->entries.size() - 1;
            }
            break;

        case HMI::down:
            // Move selection down, if possible. If at the bottom and looping is enabled, wrap to the top.
            if (currentMenu->Selection < currentMenu->entries.size() - 1) {
                ++currentMenu->Selection;
            } else if (currentMenu->behaveLoop) {
                currentMenu->Selection = 0;
            }
            break;
    }
}

    Submenu* Menu::findMenuByHeading(const std::string& heading) {
        for (auto& menu : MenuTree) {
            if (menu.getHeading() == heading) {
                return &menu;
            }
        }
        return nullptr; // Return nullptr if the menu is not found
    };

// AddMenuItem: Adds a new item to the current menu.
// - 'title' is the display text for the item.
// - 'submenu' determines if the item is a submenu or entry.
// - 'visible' determines if the item is shown in the menu.
// - 'state' is an optional toggleable state (e.g., on/off).
// - 'action' is the function to execute when the item is selected.
void Menu::AddMenuItem(const std::string& parentHeading,
                       const std::string& itemTitle,
                       bool state,
                       bool visible,
                       bool submenu,
                       std::function<void()> action) {
    // Locate the parent Submenu
    Submenu* parentMenu = findMenuByHeading(parentHeading);
    // Add Entry to the entries of this menu
    if (parentMenu) {
        parentMenu->entries.emplace_back(itemTitle, submenu, visible, state, action);
    } else {
        std::cerr << "Parent menu '" << parentHeading << "' not found." << std::endl;
    }
}

// AddSubMenu: Adds a sub-menu to the current menu.
// A "Back" option is automatically added to return to the previous menu.
void Menu::AddSubMenu(const std::string& parentHeading, const std::string& subHeading, bool loop) {
    // Locate the parent Submenu
    Submenu* parentMenu = findMenuByHeading(parentHeading);
    // Create a new submenu
    if (parentMenu) {
        MenuTree.emplace_back(parentMenu, subHeading, loop);

        // Add "Back" option
        MenuTree.back().entries.emplace_back("Back", false, true, false, [this, parentMenu]() {
            currentMenu = parentMenu;
        });

        // Add the new submenu as an entry in the parent menu
        AddMenuItem(parentHeading,
                    subHeading, 
                    true, true, false,
                    [this, submenu = &MenuTree.back()]() {
            currentMenu = submenu;
        });
    } else {
        std::cerr << "Parent menu '" << parentHeading << "' not found." << std::endl;
    }
}

// updateMenuItem: Updates an existing menu item based on its index.
// Allows changing the title, visibility, and state of the menu item.
void Menu::updateMenuItem(size_t index,
                          std::optional<bool> newState,
                          std::optional<std::string> newTitle,
                          std::optional<bool> newVisible) {
    // Ensure the index is within the range of existing menu items.
    if (index < currentMenu->entries.size()) {
        auto& item = currentMenu->entries[index];
        // Update the state, title, and visibility if new values are provided.
        if (newState.has_value()) {
            item.state = newState.value();
        }
        if (newTitle.has_value()) {
            item.title = newTitle.value();
        }
        if (newVisible.has_value()) {
            item.visible = newVisible.value();
        }
    }
}
