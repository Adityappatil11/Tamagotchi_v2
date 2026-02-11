#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "PetLogic.h"
#include <string>
#include <vector>

class CommandParser{
    public:
        // gets pointer to pet logic
        explicit CommandParser(Tamagotchi *pet): pet(pet){}

        // entry point for strings (Ai or serial buffer)
        bool ParseAndExecute(std::string input);
    private:
        Tamagotchi* pet;

        // helper to clean up strings
        std::string ToLowerCase(std::string str);
};

#endif //COMMAND_PARSER_H