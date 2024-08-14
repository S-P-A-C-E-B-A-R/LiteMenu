#include <iostream>
#include <vector>
#include <functional>
#include <string>

using namespace std;

class Menu {
    public:
        // Data Structure for an Menu Item
        struct MenuItem{
            std::string title;                      // Display Text of Menu Entry
            bool visible;                           // Menu Entry Render Status
            bool state;                             // Menu Entry Optional State
            std::function<void(MenuItem&)> action;  // Function assigned to Menu Entry
        
            // MenuItem Constructor
            MenuItem(const std::string& text, bool visible, bool state, std::function<void(MenuItem&)> action = nullptr):
                title(text), visible(visible), state(state), action(action){}
        };

        // Class Constructor
        Menu(const std::string& heading):
            heading(heading), currentLevel(0), currentSelect(0), previousSelect(0), previousMenu(nullptr)
            {}
        
        // Menu Read Only
        const std::string& getHeading() const {return heading;};
        const std::vector<MenuItem>& getEntries() const { return entries; }
        size_t getCurrentSelect() const { return currentSelect; }

        // Menu Write/Update
        void AddMenuItem(const std::string& title, bool visible, bool state, std::function<void(MenuItem&)> action = nullptr){
            entries.emplace_back(title,visible,state,action);
        };

    private:
        std::string heading;            // Menu Heading
        std::vector<MenuItem> entries;  // List Menu Entries
        std::vector<Menu> subMenus;     // List subMenus
        size_t currentLevel;            // State Menu Level
        size_t currentSelect;           // State Menu Selection
        size_t previousSelect;          // Previous Menu Selection
        Menu*  previousMenu;            // Previous Menu Level
};

// Function to toggle the status and title of a menu item
void toggleMenuItemTitle(Menu::MenuItem& item) {
    if (item.state) {
        item.title = item.title.substr(0, item.title.find(" - On")) + " - Off";
        item.state = false;
    } else {
        item.title = item.title.substr(0, item.title.find(" - Off")) + " - On";
        item.state = true;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//   Example Code Below.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup(Menu& rootMenu) {
    rootMenu.AddMenuItem("Exit/UpLevel", true, false);
    rootMenu.AddMenuItem("Toggle Option - Off", true, false, toggleMenuItemTitle);
}

void printMenu(const Menu& menu) {
    std::cout << "\n" << menu.getHeading() << "\n";
    std::cout << std::string(menu.getHeading().length(), '=') << "\n";
    
    const auto& entries = menu.getEntries();
    for (size_t i = 0; i < entries.size(); ++i) {
        if (entries[i].visible) {
            std::cout << (i == menu.getCurrentSelect() ? " > " : "   ") << entries[i].title << (entries[i].state ? " [On]" : " [Off]") << "\n";
        }
    }
}

int main()
{
    Menu rootMenu{"Main Menu"};
    setup(rootMenu);
    printMenu(rootMenu);
    
    return 0;
};