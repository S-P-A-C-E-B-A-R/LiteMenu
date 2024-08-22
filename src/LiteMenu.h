// LiteMenu.h
#ifndef LITEMENU_H
#define LITEMENU_H

#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <memory>

class LiteMenu {
private:
    struct MenuItem {
        std::string title;
        bool submenu;
        bool visible;
        bool state;
        std::function<void()> action;

        MenuItem(const std::string& text, bool submenu, bool visible, bool state, std::function<void()> action = nullptr)
            : title(text), submenu(submenu), visible(visible), state(state), action(action) {}
    };

    struct Submenu {
        std::shared_ptr<Submenu> parentMenu;
        std::string heading;
        size_t selection;
        bool behaveLoop;
        std::vector<MenuItem> entries;

        Submenu(std::shared_ptr<Submenu> parentMenu, const std::string& heading, bool behaveLoop)
            : parentMenu(parentMenu), heading(heading), selection(0), behaveLoop(behaveLoop) {}
    };

    std::vector<std::shared_ptr<Submenu>> menuTree;
    std::shared_ptr<Submenu> currentMenu;

    std::shared_ptr<Submenu> findMenuByHeading(const std::string& heading) const;
public:
    LiteMenu();
    void addMenuLevel(const std::string& parentHeading, const std::string& heading, bool loop);
    void addMenuItem(const std::string& parentHeading, const std::string& title, bool submenu, bool visible, bool state, std::function<void()> action = nullptr);
    void debugMenuTree() const;

    std::string                     getMenuTitle() const;
    size_t                          getSelection() const;
    std::vector<LiteMenu::MenuItem> getMenuEntries() const;

    // Things to API-TIZE ;-)
    void getInput();
    void toggleMenuItem();
};

#endif // LITEMENU_H
