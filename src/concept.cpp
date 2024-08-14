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
        };

        // Class Constructor
        Menu(const std::string& heading) : heading(heading), currentLevel(0), currentSelect(0), previousSelect(0), previousMenu(nullptr){};
    
        // Basic Setter/Getters
        const std::string& getHeading() const {return heading;};
    private:
        std::string heading;            // Menu Heading
        std::vector<MenuItem> entries;  // List Menu Entries
        std::vector<Menu> subMenus;     // List subMenus
        size_t currentLevel;            // State Menu Level
        size_t currentSelect;           // State Menu Selection
        size_t previousSelect;          // Previous Menu Selection
        Menu*  previousMenu;            // Previous Menu Level
};

int main()
{
    Menu rootMenu{"Main Menu"};
    cout << rootMenu.getHeading() << endl;

    return 0;
};