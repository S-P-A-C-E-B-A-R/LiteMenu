#ifndef LITEMENU_H
#define LITEMENU_H

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <optional>
#include <memory>
#include <cctype>

/**
 * @class Menu
 * 
 * @brief The Menu class represents a navigable menu system with sub-menus and menu items.
 * 
 * This class allows the creation of a hierarchical menu system where each menu can contain multiple 
 * items and sub-menus. The menu can be navigated using abstract directions (up, down, enter). 
 * Each menu item can have an associated action, visibility state, and a toggleable state. The menu 
 * system can loop at limits (e.g., navigating up from the first item brings you to the last item).
 * 
 * Usage:
 * 1. Instantiate the Menu object with a heading and looping behavior.
 * 2. Add menu items using the `AddMenuItem` method.
 * 3. Add sub-menus using the `AddSubMenu` method.
 * 4. Navigate the menu using the `navigate` method, which accepts directions like up, down, and enter.
 * 5. Update existing menu items using the `updateMenuItem` method.
 * 6. Use `getHeading` and `getEntries` to access menu information.
 */

class Menu {
private:
    std::string heading;                        ///< The title or heading of the menu.
    bool behaveLoop;                            ///< Determines if the menu loops at boundaries when navigating.
    std::vector<Menu*> subMenus;                ///< List of sub-menus under this menu.

    /**
     * @struct MenuItem
     * @brief Represents a single item within a menu.
     * 
     * Each MenuItem has a title, visibility status, toggleable state, and an associated action.
     */
    struct MenuItem {
        std::string title;                      ///< The text displayed for the menu item.
        bool submenu;                           ///< Whether the menu item is a link to a submenu.
        bool visible;                           ///< Whether the menu item is visible in the menu.
        bool state;                             ///< Optional state for the menu item (e.g., toggled on/off).
        std::function<void()> action;           ///< The function to be executed when the menu item is selected.

        /**
         * @brief Constructs a MenuItem with the given properties.
         * 
         * @param text The display text for the menu item.
         * @param visible The visibility status of the menu item.
         * @param state The toggleable state of the menu item.
         * @param action The function to be called when the menu item is selected.
         */
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

    std::vector<MenuItem> entries;  ///< List of menu items in this menu.

public:
    Menu* activeMenu;              ///< Pointer to the currently active menu (for navigation).
    size_t activeSelect;           ///< Index of the currently selected menu item.
    Menu* previousMenu;            ///< Pointer to the previously active menu.
    size_t previousSelect;         ///< Index of the previously selected menu item.

    /**
     * @enum HMI
     * @brief Abstract representation of navigation directions.
     */
    enum class HMI {
        up,    ///< Move selection up.
        down,  ///< Move selection down.
        enter  ///< Select the current menu item or enter a sub-menu.
    };

    /**
     * @brief Constructs a Menu with a specified heading and loop behavior.
     * 
     * @param heading The heading or title of the menu.
     * @param loop If true, the menu loops when navigating beyond the first or last item.
     */
    Menu(const std::string& heading,bool loop);

    /**
     * @brief Destructor for the Menu class.
     */
    ~Menu();

    /**
     * @brief Gets the heading of the menu.
     * 
     * @return The heading/title of the menu.
     */
    const std::string& getHeading() const;

    /**
     * @brief Gets the list of menu items.
     * 
     * @return A const reference to the vector of menu items.
     */
    const std::vector<MenuItem>& getEntries() const;

    /**
     * @brief Gets the index of the currently selected menu item.
     * 
     * @return The index of the currently selected menu item.
     */
    size_t getactiveSelect() const;

    /**
     * @brief Handles navigation within the menu.
     * 
     * This method updates the current selection or enters a sub-menu based on the input direction.
     * 
     * @param navigation The direction to navigate (up, down, enter).
     */
    void navigate(HMI navigation);

    /**
     * @brief Adds a new item to the menu.
     * 
     * @param title The display text for the menu item.
     * @param submenu The status as a menu item or submenu link.
     * @param visible The visibility status of the menu item.
     * @param state The initial toggleable state of the menu item.
     * @param action The function to be called when the menu item is selected.
     */
    void AddMenuItem(const std::string& title,
                     bool submenu,
                     bool visible,
                     bool state,
                     std::function<void()> action = nullptr);

    /**
     * @brief Updates an existing menu item.
     * 
     * @param index The index of the menu item to update.
     * @param newState Optional new state for the menu item.
     * @param newTitle Optional new title for the menu item.
     * @param newVisible Optional new visibility status for the menu item.
     */
    void updateMenuItem(size_t index,
                        std::optional<bool> newState = std::nullopt,
                        std::optional<std::string> newTitle = std::nullopt,
                        std::optional<bool> newVisible = std::nullopt);

    /**
     * @brief Adds a sub-menu to the current menu.
     * 
     * @param submenu Pointer to the sub-menu to be added.
     */
    void AddSubMenu(Menu* submenu);
};

#endif // LITEMENU_H
