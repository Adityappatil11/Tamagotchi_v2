#ifndef COMMAND_H
#define COMMAND_H

#include "PetLogic.h"

/**
 * @brief Base Command Interface.
 * Professional practice: This allows any sender to trigger an action 
 * without knowing the details of the Tamagotchi class.
 */
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

// --- Concrete Commands ---

class FeedCommand : public Command {
    Tamagotchi* _pet;
    float _amount;
public:
    FeedCommand(Tamagotchi* pet, float amount) : _pet(pet), _amount(amount) {}
    void execute() override { _pet->Feed(_amount); }
};

class SleepCommand : public Command {
    Tamagotchi* _pet;
    bool _state;
public:
    SleepCommand(Tamagotchi* pet, bool state) : _pet(pet), _state(state) {}
    void execute() override { _pet->Sleep(_state); }
};

class CleanCommand : public Command {
    Tamagotchi* _pet;
public:
    explicit CleanCommand(Tamagotchi* pet) : _pet(pet) {}
    void execute() override { _pet->Clean(); }
};

class PlayCommand : public Command {
    Tamagotchi* _pet;
    float _funValue;
public:
    PlayCommand(Tamagotchi* pet, float fun) : _pet(pet), _funValue(fun) {}
    void execute() override { _pet->Play(_funValue); }
};

class UseItemCommand : public Command {
    Tamagotchi* _pet;
public:
    explicit UseItemCommand(Tamagotchi* pet) : _pet(pet) {}
    
    // We'll overload execute or use a specific method for parameterized commands
    void execute() override { 
        // Default behavior if no item specified
    }

    void executeWithParam(const std::string& itemName) {
        _pet->UseItem(itemName);
    }
};

#endif // COMMAND_H