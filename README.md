# Text Menu Class for Embedded Systems

## Class Description

### Overview
The `TextMenu` class in C++ represents a text-based menu system designed for embedded devices. This class supports multiple levels of menus with headings and entries, each capable of navigating up/down through submenu levels, toggling states (On/Off), or calling external functions. It maintains the state of the current menu level and the selected item, and provides show/hide functionality for menu entries. 

### Features
- **Navigation**: Supports navigation between menu levels and entries using configurable input controls (e.g., Up/Down/Enter, Down/Enter, Short/Long Press).
- **State Management**: Remembers the current submenu level and selected item.
- **Entry Status**: Menu entries can be shown or hidden dynamically.
- **Abstract Controls**: Designed to work with any one to three input interface or display system.
- **Boundary Behavior**: Configurable behavior for scrolling past the top or bottom of the menu.

### Memory and Performance
- **Efficiency**: Optimized for memory and performance, suitable for embedded applications.
- **Bitwise Operators**: Utilizes bitwise operations to enhance performance and reduce memory usage.
- **Function Pointers**: Allows dynamic calls to functions associated with menu entries.

### Instantiation
- **Separation**: Menu instantiation is separate from the class definition to enhance ease of menu definition and management.

## Example Application & Storyboard

### Application
An embedded device with a text display and three input controls (Up, Down, Enter) will use the `TextMenu` class. The menu system should provide an intuitive navigation experience for users interacting with the device.

### Storyboard
1. **Initialization**: Device initializes and displays the main menu.
2. **Navigation**: User presses "Down" to navigate to the next item. The menu updates to highlight the selected entry.
3. **Selection**: User presses "Enter" to select an item. Depending on the item, the menu might navigate to a submenu, toggle an option, or call a function.
4. **Scrolling Behavior**: If the user scrolls past the top or bottom of the menu, predefined behavior (e.g., loop or stop) is executed.

## Design Traceability Matrix

| **User Needs**                        | **Design Requirements**                  | **Design Inputs**                       | **Design Outputs**                      | **Verification**                       | **Validation**                         |
|---------------------------------------|-----------------------------------------|----------------------------------------|----------------------------------------|---------------------------------------|----------------------------------------|
| Easy navigation of menu items         | Implement navigation controls           | User requirements for input controls   | Class methods for Up/Down/Enter controls | Unit tests for navigation functionality | User testing for intuitive navigation  |
