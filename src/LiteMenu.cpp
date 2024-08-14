#include "LiteMenu.h"

Menu::Menu(const std::string& heading, bool loop)
    : heading(heading),
      activeSelect(0),
      previousSelect(0),
      previousMenu(nullptr),
      behaveLoop(loop),
      rootMenu(this),
      activeMenu(this) {}

Menu::~Menu() {
    for (auto submenu : subMenus) {
        delete submenu;
    }
}

const std::string& Menu::getHeading() const {
    return heading;
}

const std::vector<Menu::MenuItem>& Menu::getEntries() const {
    return entries;
}

size_t Menu::getactiveSelect() const {
    return activeSelect;
}

void Menu::navigate(HMI navigation) {
    switch (navigation) {
        case HMI::enter:
            if (activeMenu->entries[activeMenu->activeSelect].action) {
                activeMenu->entries[activeMenu->activeSelect].action();
            } else {
                std::cout << "No Action" << std::endl;
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

void Menu::AddMenuItem(const std::string& title,
                      bool visible,
                      bool state,
                      std::function<void()> action) {
    activeMenu->entries.emplace_back(title, visible, state, action);
}

void Menu::AddSubMenu(const std::string& title, Menu* submenu) {
    submenu->previousMenu = activeMenu;

    submenu->AddMenuItem("Back", true, false, [this, submenu]() {
        activeMenu = submenu->previousMenu;
    });

    AddMenuItem(title, true, false, [this, submenu]() {
        activeMenu = submenu;
    });

    activeMenu->subMenus.emplace_back(submenu);
}

void Menu::updateMenuItem(size_t index,
                          std::optional<bool> newState,
                          std::optional<std::string> newTitle,
                          std::optional<bool> newVisible) {
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