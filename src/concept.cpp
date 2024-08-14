#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <optional>

using namespace std;

class Menu {
    public:
        // Data Structure for an Menu Item
        struct MenuItem{
            std::string title;                      // Display Text of Menu Entry
            bool visible;                           // Menu Entry Render Status
            bool state;                             // Menu Entry Optional State
            std::function<void()> action;           // Function assigned to Menu Entry
        
            // MenuItem Constructor
            MenuItem(const std::string& text, bool visible, bool state, std::function<void()> action = nullptr):
                title(text), visible(visible), state(state), action(action){}
        };

        // Navigate enum
        enum class HMI{
            up,
            down,
            enter
        };

        // Class Constructor
        Menu(const std::string& heading, bool loop):
            heading(heading), currentLevel(0), currentSelect(0), previousSelect(0), previousMenu(nullptr), behaveLoop(loop)
            {}

        // Class Destructor
        ~Menu() {
            // Cleanup code if needed
        }
        
        // Menu Read Only
        const std::string& getHeading() const {return heading;};
        const std::vector<MenuItem>& getEntries() const { return entries; }
        size_t getCurrentSelect() const { return currentSelect; }

        // Menu Navigation
        void navigate(HMI navigation){
            switch (navigation){
                case HMI::enter:
                    // Check if top of menu
                    if (currentSelect == 0) {
                        if (previousMenu != nullptr) {
                            currentSelect = previousMenu->previousSelect;
                            *this = *previousMenu;
                        }
                    }
                    // Check if action is associated with entrys
                    if (entries[currentSelect].action) {
                        entries[currentSelect].action();
                    }
                    break;

                case HMI::up:
                    if (currentSelect > 0) {
                        --currentSelect;
                    } else if (behaveLoop) {
                        currentSelect = entries.size() - 1;
                    }
                    break;

                case HMI::down:
                    if (currentSelect < entries.size() -1) {
                        ++currentSelect;
                    } else if (behaveLoop) {
                        currentSelect = 0;
                    }
                    break;

            }
        };

        // Menu Write/Update
        void AddMenuItem(const std::string& title, 
                         bool visible, 
                         bool state, 
                         std::function<void()> action = nullptr){
            entries.emplace_back(title,visible,state,action);
        };

        // Update Menu Item
        void updateMenuItem(size_t index, 
                            std::optional<bool> newState = std::nullopt, 
                            std::optional<std::string> newTitle = std::nullopt,
                            std::optional<bool> newVisible = std::nullopt) { 
            auto& item = entries[index];
            if (newState.has_value()) {
                item.state = newState.value();
            }
            if (newTitle.has_value()) {
                item.title = newTitle.value();
            }
            if (newVisible.has_value()){
                item.visible = newVisible.value();
            }
        };

    private:
        std::string heading;            // Menu Heading
        std::vector<MenuItem> entries;  // List Menu Entries
        std::vector<Menu> subMenus;     // List subMenus
        size_t currentLevel;            // State Menu Level
        size_t currentSelect;           // State Menu Selection
        size_t previousSelect;          // Previous Menu Selection
        Menu*  previousMenu;            // Previous Menu Level
        bool   behaveLoop;              // Loop at limits when ture
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//   Example Code Below.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <memory>
#include <cctype>

class ExampleMenuManager {
public:
    ExampleMenuManager() : rootMenu(std::make_unique<Menu>("Main Menu", false)) {
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
        rootMenu->AddMenuItem("Exit/UpLevel", true, false, [this]() { rootMenu.reset(); });
        rootMenu->AddMenuItem("Toggle Option - Off", true, false, [this]() { toggleMenuItem(); });
    }

    void printMenu() const {
        std::cout << "\n" << rootMenu->getHeading() << "\n";
        std::cout << std::string(rootMenu->getHeading().length(), '=') << "\n";
        
        const auto& entries = rootMenu->getEntries();
        for (size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].visible) {
                std::cout << (i == rootMenu->getCurrentSelect() ? " > " : "   ") 
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
                rootMenu->navigate(Menu::HMI::up);
                break;
            case 'S':
                rootMenu->navigate(Menu::HMI::down);
                break;
            case 'D':
                rootMenu->navigate(Menu::HMI::enter);
                break;
            default:
                std::cout << "Invalid input!" << std::endl;
                break;
        }

        std::cout << endl;
    }

    void toggleMenuItem() {
        // Get Entry by Index
        size_t currentIndex = rootMenu->getCurrentSelect();
        auto& item = rootMenu->getEntries()[currentIndex];
        // Update Entry Values
        std::string newTitle = item.title;
        if (item.state) {
            newTitle = newTitle.substr(0, newTitle.find(" - On")) + " - Off";
            rootMenu->updateMenuItem(currentIndex, false, newTitle);
        } else {
            newTitle = newTitle.substr(0, newTitle.find(" - Off")) + " - On";
            rootMenu->updateMenuItem(currentIndex, true, newTitle);
        }
    }
};

int main() {
    ExampleMenuManager manager;
    manager.run();
    
    return 0;
}