#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "CommandParser.h"
#include <map>
#include <string>

/**
 * @brief Translates physical button presses into application commands.
 */
class InputManager {
public:
    explicit InputManager(CommandParser* parser);

    /**
     * @brief Called by the Hardware Layer (HAL) when a button interrupt occurs.
     * @param buttonId The hardware ID of the button (e.g., 0 for Button A, 1 for Button B).
     */
    void OnButtonPressed(int buttonId);

private:
    CommandParser* _parser;
    std::map<int, std::string> _buttonMap;
};

#endif