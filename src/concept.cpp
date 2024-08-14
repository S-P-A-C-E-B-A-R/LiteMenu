#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <optional>
#include <memory>
#include <cctype>

using namespace std;

class Menu {
    private:
        Menu* rootMenu;                 // Pointer to the root menu
        std::string heading;            // Menu Heading
        std::vector<Menu*> subMenus;    // List subMenus
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

        std::vector<MenuItem> entries;  // List Menu Entries

    public:
        Menu* activeMenu;              // Currently active Menu
        size_t activeSelect;          // Currently active Selection
        Menu* previousMenu;            // Previous active Menu
        size_t previousSelect;        // Previous active Selection

        // Navigate enum
        enum class HMI {
            up,
            down,
            enter
        };

        // Class Constructor
        Menu(const std::string& heading, bool loop)
            : heading(heading),
              activeSelect(0),
              previousSelect(0),
              previousMenu(nullptr),
              behaveLoop(loop),
              rootMenu(this),
              activeMenu(this) {}

        // Class Destructor
        ~Menu() {
            for (auto submenu : subMenus) {
                delete submenu;
            }
        }

        // Menu Read Only
        const std::string& getHeading() const { return heading; }
        const std::vector<MenuItem>& getEntries() const { return entries; }
        size_t getactiveSelect() const { return activeSelect; }

        // Menu Navigation
        void navigate(HMI navigation) {
            switch (navigation) {
                case HMI::enter:
                    // Check if action is associated with entries
                    if (activeMenu->entries[activeMenu->activeSelect].action) {
                        activeMenu->entries[activeMenu->activeSelect].action();
                    } else {
                        cout << "No Action" << endl;
                    }
                    break;

                case HMI::up:
                    if (activeMenu->activeSelect > 0) {
                        --activeMenu->activeSelect;
                    } else if (activeMenu->behaveLoop) {
                        activeMenu->activeSelect = activeMenu->entries.size() - 1;
                    }
                    break;

                case HMI::down:
                    if (activeMenu->activeSelect < activeMenu->entries.size() - 1) {
                        ++activeMenu->activeSelect;
                    } else if (activeMenu->behaveLoop) {
                        activeMenu->activeSelect = 0;
                    }
                    break;
            }
        }

        // Menu Write/Update
        void AddMenuItem(const std::string& title,
                         bool visible,
                         bool state,
                         std::function<void()> action = nullptr) {
            activeMenu->entries.emplace_back(title, visible, state, action);
        }

        // Add SubMenu
        void AddSubMenu(const std::string& title, Menu* submenu) {
            // Set the current menu level as previous menu level
            submenu->previousMenu = activeMenu;

            // Add "Back" option to submenu
            submenu->AddMenuItem("Back", true, false, [this, submenu]() {
                activeMenu = submenu->previousMenu;
            });

            // Store submenu as a MenuItem with an action to navigate into it
            AddMenuItem(title, true, false, [this, submenu]() {
                activeMenu = submenu;
            });

            // Add the submenu to the list of submenus
            activeMenu->subMenus.emplace_back(submenu);
        }

        // Update Menu Item
        void updateMenuItem(size_t index,
                            std::optional<bool> newState = std::nullopt,
                            std::optional<std::string> newTitle = std::nullopt,
                            std::optional<bool> newVisible = std::nullopt) {
            if (index < activeMenu->entries.size()) {
                auto& item = activeMenu->entries[index];
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
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//   Example Code Below.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ExampleMenuManager {
public:
    ExampleMenuManager(const std::string& title, bool loop) 
        : rootMenu(std::make_unique<Menu>(title, loop)) {
        setup();
    }

    void run() {
        while (rootMenu) {
            printMenu();
            getInput();
        }
    }

private:
    std::unique_ptr<Menu> rootMenu;

    void setup() {
        rootMenu->AddMenuItem("Exit", true, false, [this]() { rootMenu.reset(); });
        rootMenu->AddMenuItem("Toggle Option - Off", true, false, [this]() { toggleMenuItem(); });

        // Create a new submenu dynamically
        auto submenuAlpha = new Menu("SubMenu Alpha", false);
        rootMenu->AddSubMenu("SubMenu Alpha", submenuAlpha);

        // Add items to the submenu
        submenuAlpha->AddMenuItem("Alpha Option", true, false);
        submenuAlpha->AddMenuItem("Alpha Option - Off", true, false, [this]() { toggleMenuItem(); });

        // Create a new submenu dynamically
        auto submenuBeta = new Menu("SubMenu Beta", false);
        rootMenu->AddSubMenu("SubMenu Beta", submenuBeta);

        // Add items to the submenu
        submenuBeta->AddMenuItem("Beta Option", true, false);
        submenuBeta->AddMenuItem("Beta Option - Off", true, false, [this]() { toggleMenuItem(); });
    }

    void printMenu() const {
        if (!rootMenu || !rootMenu->activeMenu) return;

        const auto& entries = rootMenu->activeMenu->getEntries();
        if (entries.empty()) return;

        std::cout << "\n" << rootMenu->activeMenu->getHeading() << "\n";
        std::cout << std::string(rootMenu->activeMenu->getHeading().length(), '=') << "\n";

        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].visible) {
                std::cout << (i == rootMenu->activeMenu->getactiveSelect() ? " > " : "   ")
                          << (entries[i].state ? " [T] " : " [F] ")
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
                rootMenu->activeMenu->navigate(Menu::HMI::up);
                break;
            case 'S':
                rootMenu->activeMenu->navigate(Menu::HMI::down);
                break;
            case 'D':
                rootMenu->activeMenu->navigate(Menu::HMI::enter);
                break;
            default:
                std::cout << "Invalid input!" << std::endl;
                break;
        }

        // After navigation, check if the menu has changed
        if (!rootMenu || !rootMenu->activeMenu) {
            std::cout << "Menu is no longer available!" << std::endl;
            return;
        }
    }

    void toggleMenuItem() {
        if (!rootMenu || !rootMenu->activeMenu) return;

        // Get Entry by Index
        size_t currentIndex = rootMenu->activeMenu->getactiveSelect();
        if (currentIndex >= rootMenu->activeMenu->getEntries().size()) return;

        auto& item = rootMenu->activeMenu->getEntries()[currentIndex];
        // Update Entry Values
        std::string newTitle = item.title;
        if (item.state) {
            newTitle = newTitle.substr(0, newTitle.find(" - On")) + " - Off";
            rootMenu->activeMenu->updateMenuItem(currentIndex, false, newTitle);
        } else {
            newTitle = newTitle.substr(0, newTitle.find(" - Off")) + " - On";
            rootMenu->activeMenu->updateMenuItem(currentIndex, true, newTitle);
        }
    }
};

int main() {
    ExampleMenuManager manager("Main Menu", true);
    manager.run();

    return 0;
}
