#include "CommandParser.h"
#include <algorithm>
#include <cctype>

CommandParser::CommandParser(Tamagotchi* pet) : _pet(pet) {
    // Registering synonyms for the same commands to make the AI feel natural
    _commandMap["feed"] = std::make_unique<FeedCommand>(_pet, 20.0f);
    _commandMap["eat"] = std::make_unique<FeedCommand>(_pet, 20.0f);
    
    _commandMap["sleep"] = std::make_unique<SleepCommand>(_pet, true);
    _commandMap["bed"] = std::make_unique<SleepCommand>(_pet, true);
    _commandMap["wake"] = std::make_unique<SleepCommand>(_pet, false);
    
    _commandMap["clean"] = std::make_unique<CleanCommand>(_pet);
    _commandMap["wash"] = std::make_unique<CleanCommand>(_pet);
    _commandMap["bath"] = std::make_unique<CleanCommand>(_pet);
}

bool CommandParser::ParseAndExecute(std::string input) {
    std::string cleanInput = ToLowerCase(input);

    // Look for keywords within the input string
    for (auto const& [keyword, command] : _commandMap) {
        if (cleanInput.find(keyword) != std::string::npos) {
            command->execute();
            return true;
        }
    }
    return false;
}

std::string CommandParser::ToLowerCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), 
                   [](unsigned char c){ return std::tolower(c); });
    return str;
}