#include "InputManager.h"

InputManager::InputManager(CommandParser* parser) : _parser(parser) {
    // Mapping Hardware IDs to Action Keywords
    _buttonMap[0] = "feed"; // Physical Button A
    _buttonMap[1] = "play"; // Physical Button B
    _buttonMap[2] = "clean"; // Physical Button C
}

void InputManager::OnButtonPressed(int buttonId) {
    // Look up the command associated with the button
    auto it = _buttonMap.find(buttonId);
    if (it != _buttonMap.end()) {
        // Feed the command string into the Application Layer parser
        _parser->ParseAndExecute(it->second);
    }
}