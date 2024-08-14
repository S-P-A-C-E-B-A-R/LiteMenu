#ifndef LITEMENU_H
#define LITEMENU_H

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <optional>
#include <memory>
#include <cctype>

class Menu {
private:
    Menu* rootMenu;                 // Pointer to the root menu
    std::string heading;            // Menu Heading
    std::vector<Menu*> subMenus;    // List of subMenus
    bool behaveLoop;                // Loop at limits when true

    // Data Structure for a Menu Item
    struct MenuItem {
        std::string title;                      // Display Text of Menu Entry
        bool visible;                           // Menu Entry Render Status
        bool state;                             // Menu Entry Optional State
        std::function<void()> action;           // Function assigned to Menu Entry

        // MenuItem Constructor
        MenuItem(const std::string& text, bool visible, bool state, std::function<void()> action = nullptr)
            : title(text), visible(visible), state(state), action(action) {}
    };

    std::vector<MenuItem> entries;  // List of Menu Entries

public:
    Menu* activeMenu;              // Currently active Menu
    size_t activeSelect;          // Currently active Selection
    Menu* previousMenu;           // Previous active Menu
    size_t previousSelect;        // Previous active Selection

    // Navigate enum
    enum class HMI {
        up,
        down,
        enter
    };

    // Class Constructor
    Menu(const std::string& heading, bool loop);

    // Class Destructor
    ~Menu();

    // Menu Read Only
    const std::string& getHeading() const;
    const std::vector<MenuItem>& getEntries() const;
    size_t getactiveSelect() const;

    // Menu Navigation
    void navigate(HMI navigation);

    // Menu Write/Update
    void AddMenuItem(const std::string& title,
                     bool visible,
                     bool state,
                     std::function<void()> action = nullptr);

    // Add SubMenu
    void AddSubMenu(const std::string& title, Menu* submenu);

    // Update Menu Item
    void updateMenuItem(size_t index,
                        std::optional<bool> newState = std::nullopt,
                        std::optional<std::string> newTitle = std::nullopt,
                        std::optional<bool> newVisible = std::nullopt);
};

#endif // LITEMENU_H
