#include "LiteMenu.h"

// Constructor: Initializes the Menu object with a heading and looping behavior.
// The active menu and selection are set to the current menu and the first item respectively.
Menu::Menu(const std::string& heading, bool loop)
    : heading(heading),               // Set the heading of the menu.
      activeSelect(0),                // Initialize active selection to the first item.
      previousSelect(0),              // Initialize previous selection to the first item.
      previousMenu(nullptr),          // Previous menu is initially set to null.
      behaveLoop(loop),               // Set the looping behavior (true/false).
      activeMenu(this) {}             // The active menu is the current menu instance.

// Destructor: Cleans up any sub-menus created by this menu.
// This is necessary to avoid memory leaks, as sub-menus are dynamically allocated.
Menu::~Menu() {
    for (auto submenu : subMenus) {
        delete submenu;  // Delete each sub-menu to free memory.
    }
}

// getHeading: Returns the heading/title of the menu.
// The method is const because it doesn't modify the Menu object.
const std::string& Menu::getHeading() const {
    return heading;
}

// getEntries: Returns the list of menu items.
// The method is const because it only provides read access to the entries.
const std::vector<Menu::MenuItem>& Menu::getEntries() const {
    return entries;
}

// getactiveSelect: Returns the index of the currently selected menu item.
// The method is const because it does not alter the state of the Menu object.
size_t Menu::getactiveSelect() const {
    return activeSelect;
}

// navigate: Handles menu navigation based on the provided direction.
// - 'up' moves the selection up, looping if necessary.
// - 'down' moves the selection down, looping if necessary.
// - 'enter' triggers the action associated with the currently selected menu item.
void Menu::navigate(HMI navigation) {
    switch (navigation) {
        case HMI::enter:
            // If the selected item has an action, execute it. Otherwise, print "No Action Assigned".
            if (activeMenu->entries[activeMenu->activeSelect].action) {
                activeMenu->entries[activeMenu->activeSelect].action();
            } else {
                std::cout << "No Action Assigned" << std::endl;
            }
            break;

        case HMI::up:
            // Move selection up, if possible. If at the top and looping is enabled, wrap to the bottom.
            if (activeMenu->activeSelect > 0) {
                --activeMenu->activeSelect;
            } else if (activeMenu->behaveLoop) {
                activeMenu->activeSelect = activeMenu->entries.size() - 1;
            }
            break;

        case HMI::down:
            // Move selection down, if possible. If at the bottom and looping is enabled, wrap to the top.
            if (activeMenu->activeSelect < activeMenu->entries.size() - 1) {
                ++activeMenu->activeSelect;
            } else if (activeMenu->behaveLoop) {
                activeMenu->activeSelect = 0;
            }
            break;
    }
}

// AddMenuItem: Adds a new item to the current menu.
// - 'title' is the display text for the item.
// - 'submenu' determines if the item is a submenu or entry.
// - 'visible' determines if the item is shown in the menu.
// - 'state' is an optional toggleable state (e.g., on/off).
// - 'action' is the function to execute when the item is selected.
void Menu::AddMenuItem(const std::string& title,
                      bool submenu,
                      bool visible,
                      bool state,
                      std::function<void()> action) {
    activeMenu->entries.emplace_back(title, submenu, visible, state, action);
}

// AddSubMenu: Adds a sub-menu to the current menu.
// A "Back" option is automatically added to return to the previous menu.
void Menu::AddSubMenu(Menu* submenu) {
    submenu->previousMenu = activeMenu;  // Set the previous menu of the sub-menu to the current menu.

    // Add a "Back" menu item to return to the previous menu when selected.
    submenu->AddMenuItem("Back", false, true, false, [this, submenu]() {
        activeMenu = submenu->previousMenu;
    });

    // Add the sub-menu as a selectable item in the current menu.
    AddMenuItem(submenu->getHeading(), true, true, false, [this, submenu]() {
        activeMenu = submenu;
    });

    // Store the sub-menu in the list of sub-menus for deletion in the destructor.
    activeMenu->subMenus.emplace_back(submenu);
}

// updateMenuItem: Updates an existing menu item based on its index.
// Allows changing the title, visibility, and state of the menu item.
void Menu::updateMenuItem(size_t index,
                          std::optional<bool> newState,
                          std::optional<std::string> newTitle,
                          std::optional<bool> newVisible) {
    // Ensure the index is within the range of existing menu items.
    if (index < activeMenu->entries.size()) {
        auto& item = activeMenu->entries[index];
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