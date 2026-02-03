# LiteMenu - C++ Menu Library for Embedded Systems

A lightweight, hierarchical menu system designed for embedded applications and resource-constrained environments. Complete with build system and comprehensive examples.

## Overview

The `Menu` class provides a complete text-based menu system with support for nested sub-menus, configurable navigation, and dynamic menu item management. Built with embedded systems in mind, it offers memory-efficient operation while maintaining flexibility for various use cases.

## Features

- **Hierarchical Navigation**: Multi-level menu system with automatic "Back" functionality
- **Configurable Behavior**: Optional loop-at-boundaries navigation
- **Dynamic Items**: Runtime addition and modification of menu items
- **State Management**: Toggle states for menu items (On/Off)
- **Visibility Control**: Show/hide menu items dynamically
- **Function Binding**: Associate custom actions with menu items
- **Memory Efficient**: RAII-based memory management with smart pointers
- **Cross-Platform**: Standard C++17 compatible implementation

## Quick Start

```cpp
#include "src/LiteMenu.h"

int main() {
    // Create main menu with looping enabled
    Menu mainMenu("Main Menu", true);
    
    // Add simple menu item
    mainMenu.AddMenuItem("Exit", false, true, false, []() {
        std::cout << "Exiting..." << std::endl;
    });
    
    // Add toggle-able item
    mainMenu.AddMenuItem("Settings", false, true, false, []() {
        std::cout << "Settings toggled" << std::endl;
    });
    
    // Create and add submenu
    auto* submenu = new Menu("Settings", false);
    submenu->AddMenuItem("WiFi Config", false, true, false, []() {
        std::cout << "WiFi configuration" << std::endl;
    });
    mainMenu.AddSubMenu(submenu);
    
    // Navigate menu
    mainMenu.navigate(Menu::HMI::down);
    mainMenu.navigate(Menu::HMI::enter);
    
    return 0;
}
```

## API Reference

### Constructor

```cpp
Menu(const std::string& heading, bool loop)
```

- **heading**: The title displayed for the menu
- **loop**: Enable/disable loop-at-boundaries navigation

### Methods

#### Navigation
```cpp
void navigate(HMI direction)  // Navigate: up, down, or enter
```

#### Menu Management
```cpp
void AddMenuItem(const std::string& title, bool submenu, bool visible, bool state, std::function<void()> action = nullptr)
void AddSubMenu(Menu* submenu)
void updateMenuItem(size_t index, std::optional<bool> newState, std::optional<std::string> newTitle, std::optional<bool> newVisible)
```

#### Accessors
```cpp
const std::string& getHeading() const
const std::vector<MenuItem>& getEntries() const
size_t getactiveSelect() const
```

### Menu Item Properties

Each menu item has the following properties:
- **title**: Display text
- **submenu**: Whether this item links to a submenu
- **visible**: Whether the item is currently visible
- **state**: Toggle state (true/false)
- **action**: Function to execute when selected

### Navigation Controls

The `HMI` enum provides abstract navigation:
```cpp
enum class HMI {
    up,    // Move selection up
    down,  // Move selection down  
    enter  // Select current item
}
```

## Example Implementation

See the complete working example in `examples/main.cpp` that demonstrates:

- Creating nested menu structures
- Implementing custom display logic
- Handling user input (W/S/D keys for navigation, Q to quit)
- Dynamic state updates
- Menu traversal and back navigation
- Real-time menu rendering

The example shows how to integrate the menu system into a complete interactive application with a clean console interface.

### Running the Example

```bash
# Using make
make run-debug

# Or manually
./build/debug/litemenu_demo.exe
```

## Building

### Requirements
- C++17 compatible compiler (g++, clang++, MSVC)
- Standard library
- Make (for using the provided Makefile)

### Using Makefile

The project includes a comprehensive Makefile for easy building:

```bash
# Build debug version (default)
make

# Build debug version explicitly
make debug

# Build optimized release version
make release

# Build and run
make run-debug      # Build and run debug version
make run-release    # Build and run release version

# Clean build artifacts
make clean

# Show available targets
make help
```

### Manual Compilation

```bash
# Compile the example (debug)
g++ -std=c++17 -g -I. examples/main.cpp src/LiteMenu.cpp -o menu_demo

# Compile the example (release optimized)
g++ -std=c++17 -O2 -DNDEBUG -I. examples/main.cpp src/LiteMenu.cpp -o menu_demo

# Run the demo
./menu_demo
```

## Architecture

### Memory Management
- Uses RAII principles for automatic cleanup
- Submenus are automatically deleted when parent menu is destroyed
- Smart pointer support for modern C++ applications

### Design Patterns
- **Hierarchical Composition**: Menu items and submenus form a tree structure
- **Command Pattern**: Menu actions are stored as std::function objects
- **State Pattern**: Menu maintains current selection and active menu state

### Performance Considerations
- Minimal memory footprint suitable for embedded systems
- Efficient navigation with O(1) operations for basic movement
- No dynamic allocations during navigation
- Optimized builds with -O2 flag for release deployment

### Build System
- Makefile with debug and release configurations
- Separate build directories (`build/debug`, `build/release`)
- Cross-platform compilation support (Windows .exe generation handled automatically)

## License

This project is licensed under the Apache License 2.0. See [LICENSE](LICENSE) for details.

## Contributing

Contributions are welcome! Please ensure:
- Code follows the existing style
- Documentation is updated for new features
- Examples are tested before submission

## Use Cases

### Embedded Systems
- Industrial control panels
- Consumer electronics menus
- IoT device configuration
- Medical equipment interfaces

### Applications
- Console applications
- Testing frameworks
- Configuration utilities
- Interactive tools
