#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "PetLogic.h"
#include "Command.h"
#include <string>
#include <map>
#include <memory>

class CommandParser {
public:
    explicit CommandParser(Tamagotchi* pet);
    
    // Processes a string and executes the matching command
    bool ParseAndExecute(std::string input);

private:
    Tamagotchi* _pet;
    std::map<std::string, std::unique_ptr<Command>> _commandMap;
    
    // Utility to ensure case-insensitive matching
    std::string ToLowerCase(std::string str);
};

#endif // COMMAND_PARSER_H